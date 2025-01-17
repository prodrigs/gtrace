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

// @boxes/single_gyron.hh, this file is part of gtrace.

#ifndef GTRACE_SINGLE_GYRON
#define GTRACE_SINGLE_GYRON

#include <gtrace/boxes/driver_box.hh>

/*!
Integration of a single gyron's orbit.
--------------------------------------

Sets up the `pusher_box_t`, `field_box_t`, and `observer_box_t` objects required
to follow a single gyron (ie, particle, guiding centre, etc). The actual type of
gyron to be traced is defined by the state type of the invoked `pusher_box_t`.

Driver options:

 + `-tfinal=val` Time-integration limit (default 1, in `pusher_box_t` units).
!*/
class single_gyron : public driver_box_t {
 public:
  single_gyron(int argc, char* argv[]) : driver_box_t(argc, argv) {};
  virtual ~single_gyron() {};
  virtual int operator()(int argc, char* argv[]) const;
};

#endif  // GTRACE_SINGLE_GYRON
