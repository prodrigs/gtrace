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

// @boxes/boris.cc, this file is part of gtrace.

#include <gyronimo/core/aligned_frame.hh>
#include <gyronimo/core/codata.hh>

#include <gtrace/boxes/boris.hh>

#include <iostream>

boris::boris(const settings_t& s, field_box_t* field_box)
    : settings_(s), time_step_(s.time_final / s.samples),
      stepper_(
          s.lref, s.vref, s.charge / s.mass, field_box->get_magnetic_field(),
          field_box->get_electric_field()),
      field_box_(field_box) {
  IR3 q_initial = {settings_.qu, settings_.qv, settings_.qw};
  IR3 v_initial = this->initial_velocity_from_energy_data();
  state_ = stepper_.half_back_step(q_initial, v_initial, 0.0, time_step_);
  this->print_header();
}

IR3 boris::get_dot_q(double time) const { return stepper_.get_dot_q(state_); }

IR3 boris::get_q(double time) const { return stepper_.get_position(state_); }

IR3 boris::initial_velocity_from_energy_data() const {
  using gyronimo::aligned_frame;
  using gyronimo::codata::e;
  using gyronimo::codata::m_proton;
  aligned_frame frame(stepper_.magnetic_field());
  const double energy_ref_ev =
      0.5 * m_proton * settings_.mass * settings_.vref * settings_.vref / e;
  IR3 q_initial = {settings_.qu, settings_.qv, settings_.qw};
  return frame.velocity_from_energy_data(
      {.energy = settings_.energy / energy_ref_ev,
       .pitch = settings_.pitch,
       .charge_sign = (double)(settings_.charge > 0 ? 1 : -1)},
      settings_.gyrophase, q_initial, 0);  // time=0 ok, this is initialisation!
}

boris::settings_t boris::parse_settings(const argh::parser& argh_line) {
  settings_t settings;
  argh_line("samples", 512) >> settings.samples;
  argh_line("tfinal", 1) >> settings.time_final;
  argh_line("lref", 1) >> settings.lref;
  argh_line("vref", 1) >> settings.vref;
  argh_line("mass", 1) >> settings.mass;
  argh_line("charge", 1) >> settings.charge;
  argh_line("qu", 0.1) >> settings.qu;
  argh_line("qv", 0) >> settings.qv;
  argh_line("qw", 0) >> settings.qw;
  argh_line("energy", 1) >> settings.energy;
  argh_line("pitch", 0.5) >> settings.pitch;
  argh_line("gyrophase", 0) >> settings.gyrophase;
  settings.pb = argh_line["pb"];
  settings.phires = argh_line["phires"];
  settings.pjac = argh_line["pjac"];
  settings.pkin = argh_line["pkin"];
  settings.pxyz = argh_line["pxyz"];
  return settings;
}

void boris::print_header() const {
  std::cout << "# fields: t qu qv qw vx vy vz";
  if (settings_.pxyz) std::cout << " x y z";
  if (settings_.pkin) std::cout << " Epar Eperp";
  if (settings_.pb) std::cout << " B";
  if (settings_.pjac) std::cout << " jac";
  if (settings_.phires) {
    std::cout.precision(16);
    std::cout.setf(std::ios::scientific);
  }
  std::cout << "\n";
}

void boris::print_help() {
  std::string help_message =
      "pusher_box -> gtrace::boris\n"
      "Usage: gtrace link_options -- [...] [boris options] [...]\n"
      "\n"
      "Sets up a gyronimo::classical_boris object (and all its dependencies)\n"
      "in order to push a charged particle under the Lorentz force due to the\n"
      "electromagnetic field defined some field_box (the -f link option). By\n"
      "default, the virtual method pusher_box::print_state(time) sends to\n"
      "stdout the time and the contents of boris::state_t (no newline).\n"
      "This can be tailored by additional output flags.\n"
      "Options:\n"
      "  -lref=    Reference length (in SI, default 1).\n"
      "  -vref=    Reference velocity (in SI, default 1).\n"
      "  -mass=    Particle mass (in m_proton, default 1).\n"
      "  -charge=  Particle charge (in q_proton, default 1).\n"
      "  -qu=, -qv=, -qw=\n"
      "            Initial position in the coordinates and units defined by\n"
      "            the respective field_box object.\n"
      "  -energy=, -gyrophase=, -pitch=\n"
      "            Initial energy (eV), gyrophase (rad), and pitch (v_par/v).\n"
      "  -samples= Number of time samples (tfinal/time_step, default 512).\n"
      "Options controlling the output:\n"
      "  -pb       Magnetic-field norm (in gyronimo::IR3field::m_factor).\n"
      "  -phires   Turns on high-resolution (16 digits) scientific format.\n"
      "  -pjac     Jacobian (ie, sqrt(det(g))) of the coordinate system.\n"
      "  -pkin     Parallel and perpendicular energy (in si mass*vref^2/2).\n"
      "  -pxyz     Cartesian position (si).\n";
  std::cout << help_message << std::endl;
}

void boris::print_state(double time) const {
  std::cout << time;
  for (auto s : state_) std::cout << " " << s;
  IR3 q = stepper_.get_position(state_);
  if (settings_.pxyz)
    for (auto s : (*stepper_.my_morphism())(q)) std::cout << " " << s;
  if (settings_.pkin)
    std::cout << " " << stepper_.energy_parallel(state_, time) << " "
              << stepper_.energy_perpendicular(state_, time);
  if (settings_.pb)
    std::cout << " " << stepper_.magnetic_field()->magnitude(q, time);
  if (settings_.pjac)
    std::cout << " " << stepper_.magnetic_field()->metric()->jacobian(q);
}

double boris::push_state(double time) {
  state_ = stepper_.do_step(state_, time, time_step_);
  return time + time_step_;
}
