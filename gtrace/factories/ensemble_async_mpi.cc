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

// @factories/ensemble_async_mpi.cc, this file is part of gtrace.

#include <gtrace/boxes/ensemble_async_mpi.hh>

std::unique_ptr<driver_box_t> create_linked_driver_box(int argc, char* argv[]) {
  return std::move(std::make_unique<ensemble_async_mpi>(argc, argv));
}
