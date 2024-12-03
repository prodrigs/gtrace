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

// @boxes/field_box.hh, this file is part of gtrace.

#ifndef GTRACE_FIELD_BOX
#define GTRACE_FIELD_BOX

#include <gyronimo/fields/IR3field.hh>

#include <gtrace/argh.h>

using gyronimo::IR3field;
using gyronimo::metric_covariant;

class field_box_t {
 public:
  virtual ~field_box_t() {};
  virtual const IR3field* get_electric_field() const = 0;
  virtual const IR3field* get_magnetic_field() const = 0;
  virtual const metric_covariant* get_metric() const = 0;
  bool is_metric_consistent() const;
};

inline bool field_box_t::is_metric_consistent() const {
  const IR3field* E = this->get_electric_field();
  const IR3field* B = this->get_magnetic_field();
  return (E && B ? E->metric() == B->metric() : true);
}

field_box_t* create_linked_field_box(const argh::parser& command_line);

#endif  // GTRACE_FIELD_BOX
