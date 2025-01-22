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

// @boxes/q_predicate.hh, this file is part of gtrace.

#ifndef GTRACE_Q_PREDICATE
#define GTRACE_Q_PREDICATE

#include <gtrace/boxes/step_printer.hh>

#include <memory>

/*!
Conditional integration via a gyron-position predicate.
-------------------------------------------------------

Breaks time integration if the gyron's position moves outside the bounds
established by the supplied options. These coordinate bounds are to be
interpreted in terms of the relevant `field_box_t` object. By default, the
virtual member `pusher_box_t::print_state(time)` is invoked at `time=0` (if the
initial position lies within bounds) and at the first time step after
`time<tfinal` or after the gyron's position moves out of bounds.  Alternatively,
the option `-step-mode` forces a `step_printer` observer to be built and invokes
it at every time step within the requested bounds (all step_printer options
apply).

Observer options:

 + `-qumin=val, -qumax=val` Position limits (default lowest/largest double).
 + `-step-mode` Builds a `step_printer` observer and invokes it within bounds.
!*/
class q_predicate : public observer_box_t {
 public:
  q_predicate() = delete;
  q_predicate(const argh::parser& argh_line, std::ostream& os);
  virtual ~q_predicate() {};
  virtual bool operator()(
      const pusher_box_t* pusher, double time) const override;
 private:
  const bool is_step_mode_;
  double qu_min_, qu_max_;
  double tfinal_;
  std::unique_ptr<step_printer> step_printer_;
  bool invoke_step_mode(const pusher_box_t* pusher, double time) const;
  bool invoke_default(const pusher_box_t* pusher, double time) const;
  bool is_within_bounds(const IR3& q) const;
  void print_last_state(const pusher_box_t* pusher, double time) const;
};

#endif  // GTRACE_Q_PREDICATE
