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

// @factories/littlejohn1983.cc, this file is part of gtrace.

#include <gtrace/boxes/littlejohn1983.hh>

#include <iostream>

pusher_box_t* create_linked_pusher_box(const argh::parser& argh_line) {
  field_box_t* field_box = create_linked_field_box(argh_line);
  if (argh_line[{"h", "help"}]) {
    littlejohn1983::print_help();
    return nullptr;
  }
  if (!field_box) {
    std::cerr << "gtrace::littlejohn1983: empty box, nothing to be done.\n";
    return nullptr;
  }
  if (!field_box->is_metric_consistent()) {
    std::cerr << "gtrace::littlejohn1983: inconsistent field box." << std::endl;
    return nullptr;
  }
  littlejohn1983::settings_t settings =
      littlejohn1983::parse_settings(argh_line);
  return new littlejohn1983(settings, field_box);
}
