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

// @boxes/q_predicate.cc, this file is part of gtrace.

#include <gtrace/boxes/q_predicate.hh>

#include <iostream>
#include <limits>

bool q_predicate::invoke_default(
    const pusher_box_t* pusher, double time) const {
  IR3 q = pusher->get_q(time);
  if (this->is_within_bounds(q)) {
    if (time == 0) pusher->print_state(time);
    if (time >= tfinal_) this->print_last_state(pusher, time);
    return true;
  } else {
    if (time > 0) this->print_last_state(pusher, time);
    return false;
  }
}

bool q_predicate::invoke_step_mode(
    const pusher_box_t* pusher, double time) const {
  IR3 q = pusher->get_q(time);
  if (this->is_within_bounds(q)) {
    (*step_printer_.get())(pusher, time);
    return true;
  } else return false;
}

bool q_predicate::is_within_bounds(const IR3& q) const {
  return q[IR3::u] >= qu_min_ && q[IR3::u] <= qu_max_;
}

bool q_predicate::operator()(const pusher_box_t* pusher, double time) const {
  if (is_step_mode_) return this->invoke_step_mode(pusher, time);
  else return this->invoke_default(pusher, time);
}

void q_predicate::print_last_state(
    const pusher_box_t* pusher, double time) const {
  std::cout << " ";
  pusher->print_state(time);
  std::cout << "\n";
}

q_predicate::q_predicate(const argh::parser& argh_line)
    : step_printer_(nullptr), is_step_mode_(argh_line["step-mode"]) {
  if (is_step_mode_) step_printer_ = std::make_unique<step_printer>(argh_line);
  argh_line("qumin", std::numeric_limits<double>::lowest()) >> qu_min_;
  argh_line("qumax", std::numeric_limits<double>::max()) >> qu_max_;
  argh_line("tfinal", 1) >> tfinal_;
}
