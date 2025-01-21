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

// @boxes/boris.hh, this file is part of gtrace.

#ifndef GTRACE_BORIS
#define GTRACE_BORIS

#include <gyronimo/dynamics/classical_boris.hh>

#include <gtrace/boxes/pusher_box.hh>

#include <memory>

using gyronimo::classical_boris;

/*!
Classical Boris pusher for charged particles in electromagnetic fields.
-----------------------------------------------------------------------

Sets up a `gyronimo::classical_boris` object (and all its dependencies) in order
to push a charged particle under the Lorentz force due to the electromagnetic
field defined by some `field_box_t` (as directed by the -f link option). Both
the pusher and the field objects are built from the options supplied in the
argument list. By default, the overriden virtual method
`boris::print_state(time)` sends to the output stream the time and the contents
of `boris::state_t` (no newline). This can be tailored by additional output
flags.

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

Options controlling the output:

 + `-pb` Magnetic-field norm (in `gyronimo::IR3field::m_factor` units).
 + `-phires` Turns on high-resolution (16 digits) scientific format.
 + `-pjac` Jacobian (ie, $\sqrt{\det(g)}$ of the coordinate system.
 + `-pkin` Parallel and perpendicular energy (in si `mass*vref^2/2`).
 + `-pxyz` Cartesian position (SI).
!*/
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
  boris(const settings_t& settings, const field_box_t* field_box);
  virtual ~boris() {};
  virtual IR3 get_dot_q(double time) const override;
  virtual IR3 get_q(double time) const override;
  virtual void print_state(double time) const override;
  virtual double push_state(double time) override;
 private:
  const double time_step_;
  const settings_t settings_;
  const classical_boris stepper_;
  const field_box_t* field_box_;
  state_t state_;
  void print_header() const;
  IR3 initial_velocity_from_energy_data() const;
};

#endif  // GTRACE_BORIS
