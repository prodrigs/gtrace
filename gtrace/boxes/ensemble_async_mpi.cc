// gtrace -- a flexible gyron-tracing application for electromagnetic fields.
// Copyright (C) 2024 Paulo Rodrigues.

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

// @boxes/ensemble_async_mpi.cc, this file is part of gtrace.

#include <gtrace/boxes/ensemble_async_mpi.hh>

#include <memory>
#include <mpi.h>

ensemble_async_mpi::ensemble_async_mpi(int argc, char* argv[])
    : driver_box_t(argc, argv) {
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &mpi_rank_);
  MPI_Comm_size(MPI_COMM_WORLD, &mpi_size_);
}

ensemble_async_mpi::~ensemble_async_mpi() { MPI_Finalize(); }

int ensemble_async_mpi::operator()(int argc, char* argv[]) const {
  auto argh_line = this->argh_line();
  if (argh_line[{"h", "help"}]) {
    if (mpi_rank_ == 0) this->print_help();
    return 1;
  }

  auto [in_stream, in_filename] = this->get_input_stream(argh_line, mpi_rank_);
  stream_redirector redirected_in_scope(std::cout, in_filename + ".cout");

  this->print_header(argc, argv);
  auto shared_options = this->convert_argv_to_string(argv);

  for (std::string private_options; std::getline(in_stream, private_options);) {
    auto full_argh = argh::parser(shared_options + private_options);
    std::unique_ptr<pusher_box_t> pusher {create_linked_pusher_box(full_argh)};
    std::unique_ptr<observer_box_t> observer {
        create_linked_observer_box(full_argh)};

    double time_final;
    full_argh("tfinal", 1) >> time_final;
    this->integrate_orbit(pusher.get(), observer.get(), time_final);
  }

  in_stream.close();
  return 0;
}

void ensemble_async_mpi::print_help() {
  driver_box_t::print_version();
  std::string usage_message =
      "\n"
      "driver_box -> gtrace::ensemble_async_mpi\n"
      "Usage: gtrace link_options -- -prefix= [-tfinal=] [shared_options]\n"
      "\n"
      "Integrates a collection (ensemble) of gyrons (ie, particles, guiding\n"
      "centres, etc) much like in gtrace::ensemble_async (more details in its\n"
      "documentation). However, the full ensemble is here split by a number\n"
      "of mpi parallel processes, each one integrating asynchronously its\n"
      "corresponding sub-ensemble. There must be as many input files as\n"
      "running processes, each one named as prefix-nnn, with nnn the process\n"
      "rank number. The output printed by the pusher_box to std::cout is\n"
      "redirected to files named prefix-nnn.cout.\n"
      "Options:\n"
      "  -prefix=  Prefix of input & output filenames. Input files must be\n"
      "            named as prefix-nnn (nnn being the process rank), the\n"
      "            corresponding output being stored in prefix-nnn.cout.\n"
      "  -tfinal=  Time-integration limit (default 1, in pusher_box units).\n";
  std::cout << usage_message << std::endl;
  driver_box_t::print_help();

  auto help_argh = argh::parser("-help");
  pusher_box_t* dummy_pusher = create_linked_pusher_box(help_argh);
  if (dummy_pusher) delete dummy_pusher;
  observer_box_t* dummy_observer = create_linked_observer_box(help_argh);
  if (dummy_observer) delete dummy_observer;
}

std::string ensemble_async_mpi::convert_argv_to_string(char* argv[]) const {
  std::ostringstream stream;
  for (auto p = argv; *p; ++p) stream << *p << " ";
  return stream.str();
}

std::pair<std::ifstream, std::string> ensemble_async_mpi::get_input_stream(
    const argh::parser& argh_line, int mpi_rank) const {
  std::string filename;
  argh_line("prefix", "") >> filename;
  filename += "-" + std::to_string(mpi_rank);
  std::ifstream in_stream(filename);
  if (!in_stream.is_open()) {
    std::cerr << "gtrace::ensemble_async_mpi: cannot open file \"" << filename
              << "\" for reading." << std::endl;
    std::exit(1);
  }
  return {std::move(in_stream), filename};
}
