// gtrace -- a flexible gyron-tracing application for electromagnetic fields.
// Copyright (C) 2025 Paulo Rodrigues.

// gtrace is free software: you can redistribute it and/or modify it
// under the terms of the GNU General Public License as published by the
// Free Software Foundation, either version 3 of the License, or (at
// your option) any later version.

// gtrace is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
// FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License
// for more details.

// You should have received a copy of the GNU General Public License
// along with gtrace. If not, see <https://www.gnu.org/licenses/>.

// @boxes/ensemble_async.cc, this file is part of gtrace.

#include <gtrace/boxes/ensemble_async.hh>

#include <execution>
#include <iostream>
#include <syncstream>

auto ensemble_async::get_boxes(
    const argh::parser& arghs, std::ostream& os) const {
  auto field = create_linked_field_box(arghs);
  auto pusher = create_linked_pusher_box(arghs, field.get());
  auto observer = create_linked_observer_box(arghs, os);
  return std::tuple(std::move(field), std::move(pusher), std::move(observer));
}

std::vector<std::string> ensemble_async::get_option_lines_from_file(
    const argh::parser& arghs) const {
  std::string filename;
  arghs("ensemble-file", "") >> filename;
  std::ifstream in_stream(filename);
  if (!in_stream.is_open())
    throw std::runtime_error("cannot read from file " + filename + ".\n");

  std::vector<std::string> option_lines;
  for (std::string line; std::getline(in_stream, line);)
    option_lines.emplace_back(line);
  return option_lines;
}

int ensemble_async::operator()(int argc, char* argv[]) const {
  std::cout << this->header_string(argc, argv) << "\n";
  if (argh_line_["sci-16"]) {
    std::cout.precision(16);
    std::cout.setf(std::ios::scientific);
  }
  std::string shared_options = this->convert_argv_to_string(argv);
  auto private_option_lines = this->get_option_lines_from_file(argh_line_);
  std::for_each(
      std::execution::par, private_option_lines.begin(),
      private_option_lines.end(),
      [&shared_options, this](const std::string& private_options) {
        std::osyncstream out_stream(std::cout);
        auto arghs = argh::parser(shared_options + private_options);
        auto [field, pusher, observer] = this->get_boxes(arghs, out_stream);
        out_stream << pusher->compose_output_fields() << "\n";

        double time_final;
        arghs("tfinal", 1) >> time_final;
        std::string elapsed_time_info =
            this->integrate_orbit(pusher.get(), observer.get(), time_final);
        if (argh_line_["elapsed-time"]) out_stream << elapsed_time_info << "\n";
      });
  return 0;
}

std::string ensemble_async::convert_argv_to_string(char* argv[]) const {
  std::ostringstream stream;
  for (auto p = argv; *p; ++p) stream << *p << " ";
  return stream.str();
}
