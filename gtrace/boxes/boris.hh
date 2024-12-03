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

// @boxes/boris.hh, this file is part of gtrace.

#ifndef GTRACE_BORIS
#define GTRACE_BORIS

#include <gyronimo/dynamics/classical_boris.hh>

#include <gtrace/boxes/field_box.hh>
#include <gtrace/boxes/pusher_box.hh>

#include <memory>

using gyronimo::classical_boris;

class boris : public pusher_box_t {
 public:
  using state_t = classical_boris::state;
  struct settings_t {
    size_t samples;
    double charge, lref, mass, time_final, vref;
    double qu, qv, qw, energy, gyrophase, pitch;
    bool pb, phires, pjac, pkin, pxyz;
  };
  static settings_t parse_settings(const argh::parser& argh_line);
  static void print_help();
  boris(const settings_t& settings, field_box_t* field_box);
  virtual ~boris() {};
  virtual IR3 get_dot_q(double time) const override;
  virtual IR3 get_q(double time) const override;
  virtual void print_state(double time) const override;
  virtual double push_state(double time) override;
 private:
  const double time_step_;
  const settings_t settings_;
  const classical_boris stepper_;
  const std::unique_ptr<field_box_t> field_box_;
  state_t state_;
  void print_header() const;
  IR3 initial_velocity_from_energy_data() const;
};

#endif  // GTRACE_BORIS
