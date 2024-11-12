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

// @boxes/single_gyron.hh, this file is part of gtrace.

#ifndef GTRACE_SINGLE_GYRON
#define GTRACE_SINGLE_GYRON

#include <gtrace/boxes/driver_box.hh>

class single_gyron : public driver_box_t {
 public:
  static void print_help();
  single_gyron(int argc, char* argv[]) : driver_box_t(argc, argv) {};
  virtual ~single_gyron() {};
  virtual int operator()(int argc, char* argv[]) const;
};

#endif  // GTRACE_SINGLE_GYRON
