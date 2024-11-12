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

// @boxes/step_printer.cc, this file is part of gtrace.

#include <gtrace/boxes/step_printer.hh>

#include <iostream>

step_printer::step_printer(const argh::parser& argh_line) {
  argh_line("skip", 0) >> skip_;
  skipped_steps_ = (argh_line["skip-initial"] ? 0 : skip_);
}

bool step_printer::operator()(const pusher_box_t* pusher, double time) const {
  if (skipped_steps_ < skip_) skipped_steps_++;
  else {
    pusher->print_state(time);
    std::cout << "\n";
    skipped_steps_ = 0;
  }
  return true;
}

void step_printer::print_help() {
  std::string help_message =
      "observer_box -> gtrace::step_printer\n"
      "Usage: gtrace link_options -- [...] [step_printer options] [...]\n"
      "\n"
      "Calls pusher->print_state(time) sequentially throughout the entire\n"
      "integration process (provides no termination condition by itself),\n"
      "eventually skipping skip time steps.\n"
      "Options:\n"
      "  -skip=   Number of time steps to be skipped (default 0).\n"
      "  -skip-initial\n"
      "           Skips also the step corresponding to time=0.\n";
  std::cout << help_message << std::endl;
}
