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

// @boxes/driver_box.hh, this file is part of gtrace.

#ifndef GTRACE_DRIVER_BOX
#define GTRACE_DRIVER_BOX

#include <gtrace/boxes/observer_box.hh>
#include <gtrace/boxes/pusher_box.hh>
#include <gtrace/tools/argh.h>

#include <string>

/*!
Base class for gyron-integration drivers.
-----------------------------------------

Options:

 + `-peek-beyond-tfinal`\
    Invokes the observer also on the state that is pushed one time step beyond
    the integration limit `tfinal`.
!*/
class driver_box_t {
 public:
  static std::string header_string(int argc, char* argv[]);
  driver_box_t() = delete;
  driver_box_t(int argc, char* argv[]);
  virtual ~driver_box_t() {};
  virtual int operator()(int argc, char* argv[]) const = 0;
  virtual std::string integrate_orbit(
      pusher_box_t* pusher, const observer_box_t* observer,
      double tfinal) const;
 protected:
  const argh::parser argh_line_;
};

driver_box_t* create_linked_driver_box(int argc, char* argv[]);

#endif  // GTRACE_DRIVER_BOX
