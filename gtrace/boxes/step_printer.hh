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

// @boxes/step_printer.hh, this file is part of gtrace.

#ifndef GTRACE_STEP_PRINTER
#define GTRACE_STEP_PRINTER

#include <gtrace/boxes/observer_box.hh>

class step_printer : public observer_box_t {
 public:
  static void print_help();
  step_printer(const argh::parser& argh_line);
  virtual ~step_printer() {};
  virtual bool operator()(
      const pusher_box_t* pusher, double time) const override;
 private:
  size_t skip_;
  mutable size_t skipped_steps_;
};

#endif  // GTRACE_STEP_PRINTER
