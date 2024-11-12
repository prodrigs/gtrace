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

// @factories/vmec_b.cc, this file is part of gtrace.

#include <gtrace/boxes/vmec_b.hh>

field_box_t* create_linked_field_box(const argh::parser& argh_line) {
  if (argh_line[{"h", "help"}]) {
    vmec_b::print_help();
    return nullptr;
  }
  return new vmec_b(argh_line);
}
