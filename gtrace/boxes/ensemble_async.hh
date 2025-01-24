// gtrace -- a flexible gyron-tracing application for electromagnetic fields.
// Copyright (C) 2025 Paulo Rodrigues.

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

// @boxes/ensemble_async.hh, this file is part of gtrace.

#ifndef GTRACE_ENSEMBLE_ASYNC
#define GTRACE_ENSEMBLE_ASYNC

#include <gtrace/boxes/driver_box.hh>

#include <fstream>
#include <string>
#include <vector>

/*!
Asynchronous integration of a gyron ensemble.
---------------------------------------------

Integrates a collection (ensemble) of gyrons (ie, particles, guiding centres,
etc) as defined by the state type of the invoked pusher_box. The integration is
performed asynchronously (one gyron at a time) and individual pusher objects for
each gyron in the collection are built from the concatenation of the options
supplied at the command line (ie, the shared_options) with those at each line of
the input file. If available, the `c++17` execution policy `std::execution::par`
is employed to split the ensemble integration in parallel tasks.

Driver options:

 + `-ensemble-file=val` Path to the input file, one set of options per line.
 + `-tfinal=val` Time-integration limit (default 1, in `pusher_box_t` units).
!*/
class ensemble_async : public driver_box_t {
 public:
  ensemble_async(int argc, char* argv[]) : driver_box_t(argc, argv) {};
  virtual ~ensemble_async() {};
  virtual int operator()(int argc, char* argv[]) const;
 private:
  std::string convert_argv_to_string(char* argv[]) const;
  auto get_boxes(const argh::parser& arghs, std::ostream& os) const;
  std::vector<std::string> get_option_lines_from_file(
      const argh::parser& arghs) const;
};

#endif  // GTRACE_ENSEMBLE_ASYNC
