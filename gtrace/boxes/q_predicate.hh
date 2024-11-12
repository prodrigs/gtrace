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

// @boxes/q_predicate.hh, this file is part of gtrace.

#ifndef GTRACE_Q_PREDICATE
#define GTRACE_Q_PREDICATE

#include <gtrace/boxes/step_printer.hh>

#include <memory>

class q_predicate : public observer_box_t {
 public:
  static void print_help();
  q_predicate(const argh::parser& argh_line);
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
