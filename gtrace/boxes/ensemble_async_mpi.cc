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
