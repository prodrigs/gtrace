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

// @boxes/littlejohn1983.hh, this file is part of gtrace.

#ifndef GTRACE_LITTLEJOHN1983
#define GTRACE_LITTLEJOHN1983

#include <gyronimo/dynamics/guiding_centre.hh>

#include <gtrace/boxes/field_box.hh>
#include <gtrace/boxes/pusher_box.hh>
#include <gtrace/tools/odeint_stepper.hh>

#include <memory>

using gyronimo::guiding_centre;

class littlejohn1983 : public pusher_box_t {
 public:
  using state_t = guiding_centre::state;
  struct settings_t {
    size_t samples;
    double charge, lref, mass, time_final, vref;
    double qu, qv, qw, energy, gyrophase, pitch;
    bool pb, phires, pjac, pkin, pxyz;
    std::string odeint;
  };
  static settings_t parse_settings(const argh::parser& argh_line);
  static void print_help();

  littlejohn1983(const settings_t& settings, const field_box_t* field_box);
  virtual ~littlejohn1983() {};
  virtual IR3 get_dot_q(double time) const override;
  virtual IR3 get_q(double time) const override;
  virtual void print_state(double time) const override;
  virtual double push_state(double time) override {
    return (*stepper_).do_step(eqs_motion_, state_, time, time_step_);
  };
 private:
  const double time_step_;
  const settings_t settings_;
  const guiding_centre eqs_motion_;
  const std::unique_ptr<const field_box_t> field_box_;
  const std::unique_ptr<odeint_stepper<guiding_centre>> stepper_;
  state_t state_;

  static double get_energy_tilde(const settings_t& s);
  static double get_mu_tilde(const settings_t& s, const field_box_t* fb);
  static bool is_pxyz_inconsistent(const settings_t& s, const field_box_t* fb);
  void print_header() const;
};

#endif  // GTRACE_LITTLEJOHN1983
