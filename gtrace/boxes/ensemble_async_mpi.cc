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
  auto [in_stream, out_stream] = this->get_io_streams(argh_line_, mpi_rank_);
  out_stream << this->header_string(argc, argv) << "\n";

  auto shared_options = this->convert_argv_to_string(argv);

  for (std::string private_options; std::getline(in_stream, private_options);) {
    auto full_argh = argh::parser(shared_options + private_options);
    std::unique_ptr<field_box_t> field {create_linked_field_box(full_argh)};
    std::unique_ptr<pusher_box_t> pusher {
        create_linked_pusher_box(full_argh, field.get())};
    std::unique_ptr<observer_box_t> observer {
        create_linked_observer_box(full_argh, out_stream)};

    out_stream << pusher->compose_output_fields() << "\n";
    if (argh_line_["sci-16"]) {
      out_stream.precision(16);
      out_stream.setf(std::ios::scientific);
    }

    double time_final;
    full_argh("tfinal", 1) >> time_final;
    std::string elapsed_time_info =
        this->integrate_orbit(pusher.get(), observer.get(), time_final);
    if (argh_line_["elapsed-time"]) out_stream << elapsed_time_info << "\n";
  }

  out_stream.close();
  in_stream.close();
  return 0;
}

std::string ensemble_async_mpi::convert_argv_to_string(char* argv[]) const {
  std::ostringstream stream;
  for (auto p = argv; *p; ++p) stream << *p << " ";
  return stream.str();
}

std::pair<std::ifstream, std::ofstream> ensemble_async_mpi::get_io_streams(
    const argh::parser& argh_line, int mpi_rank) const {
  std::string filename;
  argh_line("prefix", "") >> filename;
  filename += "-" + std::to_string(mpi_rank);
  std::ifstream in_stream(filename);
  if (!in_stream.is_open())
    throw std::runtime_error("cannot read from file " + filename + ".\n");
  std::ofstream out_stream(filename + ".cout");
  if (!out_stream.is_open())
    throw std::runtime_error("cannot write to file " + filename + ".cout.\n");
  return {std::move(in_stream), std::move(out_stream)};
}
