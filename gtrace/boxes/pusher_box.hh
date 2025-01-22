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

// @boxes/pusher_box.hh, this file is part of gtrace.

#ifndef GTRACE_PUSHER_BOX
#define GTRACE_PUSHER_BOX

#include <gyronimo/core/IR3algebra.hh>

#include <gtrace/boxes/field_box.hh>

#include <list>

using gyronimo::IR3;

class pusher_box_t {
 public:
  pusher_box_t() = delete;
  pusher_box_t(const field_box_t* field_box);
  virtual ~pusher_box_t() {};
  virtual double push_state(double time) = 0;
  virtual IR3 get_q(double time) const = 0;
  virtual IR3 get_dot_q(double time) const = 0;
  virtual std::string compose_output_fields() const = 0;
  virtual std::list<double> compose_output_values(double time) const = 0;
 protected:
  const field_box_t* const field_box_;
};

std::unique_ptr<pusher_box_t> create_linked_pusher_box(
    const argh::parser& arghs, const field_box_t* field_box);

#endif  // GTRACE_PUSHER_BOX
