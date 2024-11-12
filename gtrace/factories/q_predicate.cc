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

// @factories/q_predicate.cc, this file is part of gtrace.

#include <gtrace/boxes/q_predicate.hh>

observer_box_t* create_linked_observer_box(const argh::parser& argh_line) {
  if (argh_line[{"h", "help"}]) {
    q_predicate::print_help();
    return nullptr;
  }
  return new q_predicate(argh_line);
}
