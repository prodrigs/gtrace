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
  auto argh_line = this->argh_line();
  this->print_header(argc, argv);
  std::unique_ptr<pusher_box_t> pusher {create_linked_pusher_box(argh_line)};
  std::unique_ptr<observer_box_t> observer {
      create_linked_observer_box(argh_line)};

  double time_final;
  argh_line("tfinal", 1) >> time_final;
  this->integrate_orbit(pusher.get(), observer.get(), time_final);
  return 0;
}
