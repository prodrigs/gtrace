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

// @boxes/step_printer.cc, this file is part of gtrace.

#include <gtrace/boxes/step_printer.hh>

#include <iostream>

step_printer::step_printer(const argh::parser& arghs, std::ostream& os)
    : observer_box_t(os) {
  arghs("skip", 0) >> skip_;
  skipped_steps_ = (arghs["skip-initial"] ? 0 : skip_);
}

bool step_printer::operator()(const pusher_box_t* pusher, double time) const {
  if (skipped_steps_ < skip_) skipped_steps_++;
  else {
    for (auto x : pusher->compose_output_values(time)) ostream_ << x << " ";
    ostream_ << "\n";
    skipped_steps_ = 0;
  }
  return true;
}
