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

// @boxes/observer_box.hh, this file is part of gtrace.

#ifndef GTRACE_OBSERVER_BOX
#define GTRACE_OBSERVER_BOX

#include <gtrace/boxes/pusher_box.hh>

class observer_box_t {
 public:
  observer_box_t() = delete;
  observer_box_t(std::ostream& os) : ostream_(os) {};
  virtual ~observer_box_t() {};
  virtual bool operator()(const pusher_box_t* pusher, double time) const = 0;
 protected:
  std::ostream& ostream_;
};

observer_box_t* create_linked_observer_box(
    const argh::parser& argh_line, std::ostream& os);

#endif  // GTRACE_OBSERVER_BOX
