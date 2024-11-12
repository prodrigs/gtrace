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

// @boxes/pusher_box.hh, this file is part of gtrace.

#ifndef GTRACE_PUSHER_BOX
#define GTRACE_PUSHER_BOX

#include <gyronimo/core/IR3algebra.hh>

#include <gtrace/argh.h>

using gyronimo::IR3;

class pusher_box_t {
 public:
  virtual ~pusher_box_t() {};
  virtual IR3 get_q() const = 0;
  virtual IR3 get_dot_q() const = 0;
  virtual void print_state(double time) const = 0;
  virtual double push_state(double time) = 0;
};

pusher_box_t* create_linked_pusher_box(const argh::parser& command_line);

#endif  // GTRACE_PUSHER_BOX
