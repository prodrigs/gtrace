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

// @boxes/littlejohn1983.hh, this file is part of gtrace.

#ifndef GTRACE_LITTLEJOHN1983
#define GTRACE_LITTLEJOHN1983

#include <gyronimo/dynamics/guiding_centre.hh>

#include <gtrace/boxes/field_box.hh>
#include <gtrace/boxes/pusher_box.hh>
#include <gtrace/tools/odeint_stepper.hh>

#include <memory>

using gyronimo::guiding_centre;

/*!
Pusher for guiding-centre motion powered by `boost::odeint` algorithms.
-----------------------------------------------------------------------

Sets up a `gyronimo::guiding_centre object` (and its dependencies) to be traced
along an electromagnetic field defined by some `field_box_t` object employing an
available [`boost::odeint`](https://www.boost.org) algorithm for ODE
integration. By default, the overriden virtual method
`pusher_box_t::print_state(time)` prints to the output stream the time and
contents of `littlejohn1983::state_t` (no newline). This can be tailored by
additional output flags.

Pusher options:

 + `-lref=val, -vref=val`\
    Reference length and velocity for normalisation (in SI units, default 1).

 + `-mass=val, -charge=val`\
    Particle mass and charge (in m_proton and q_proton, default 1).

 + `-qu=val, -qv=val, -qw=val`\
    Initial position in the coordinate system and units as defined by the
    respective `field_box_t` object.

 + `-energy=val, -gyrophase=val, -pitch=val`\
    Initial kinetic energy (eV), gyrophase (rad), and pitch (ie., the ratio
    $v_\parallel/v$).

 + `-samples=val` Number of time samples (`tfinal/time_step`, default 512).
 + `-odeint={adams|fehlberg|rungekutta}` ODE algorithm (defaults to rungekutta).

Options controlling the output:

 + `-pb` Magnetic-field norm (normalised to 'gyronimo::IR3field::m_factor').
 + `-phires` Turns on high-resolution (16 digits) scientific format.
 + `-pjac` Jacobian (ie, $\sqrt{\det(g)}$ of the coordinate system.
 + `-pkin` Parallel and perpendicular energy (normalised to `mass*vref^2/2').
 + `-pxyz` Cartesian position (SI, for connected metrics only).
!*/
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
