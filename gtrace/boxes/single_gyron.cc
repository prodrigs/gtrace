// gtrace -- a flexible gyron-tracing application for electromagnetic fields.
// Copyright (C) 2024-2025 Paulo Rodrigues.

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

// @boxes/single_gyron.cc, this file is part of gtrace.

#include <gtrace/boxes/single_gyron.hh>

#include <iostream>
#include <memory>

int single_gyron::operator()(int argc, char* argv[]) const {
  std::unique_ptr<field_box_t> field {create_linked_field_box(argh_line_)};
  std::unique_ptr<pusher_box_t> pusher {
      create_linked_pusher_box(argh_line_, field.get())};
  std::unique_ptr<observer_box_t> observer {
      create_linked_observer_box(argh_line_, std::cout)};

  std::cout << this->header_string(argc, argv) << "\n"
            << pusher->compose_output_fields() << "\n";
  if (argh_line_["sci-16"]) {
    std::cout.precision(16);
    std::cout.setf(std::ios::scientific);
  }

  double time_final;
  argh_line_("tfinal", 1) >> time_final;
  std::string elapsed_time_info =
      this->integrate_orbit(pusher.get(), observer.get(), time_final);
  if (argh_line_["elapsed-time"]) std::cout << elapsed_time_info << "\n";
  return 0;
}
