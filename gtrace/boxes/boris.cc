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

// @boxes/boris.cc, this file is part of gtrace.

#include <gyronimo/core/aligned_frame.hh>
#include <gyronimo/core/codata.hh>

#include <gtrace/boxes/boris.hh>

#include <algorithm>

boris::boris(const settings_t& s, const field_box_t* field_box)
    : pusher_box_t(field_box), settings_(s),
      time_step_(s.time_final / s.samples),
      stepper_(
          s.lref, s.vref, s.charge / s.mass, field_box->get_magnetic_field(),
          field_box->get_electric_field()),
      field_box_(field_box) {
  IR3 q_initial = {settings_.qu, settings_.qv, settings_.qw};
  IR3 v_initial = this->initial_velocity_from_energy_data();
  state_ = stepper_.half_back_step(q_initial, v_initial, 0.0, time_step_);
}

std::string boris::compose_output_fields() const {
  std::string output_fields("# fields: t qu qv qw vx vy vz");
  if (settings_.pxyz) output_fields += " x y z";
  if (settings_.pkin) output_fields += " Epar Eperp";
  if (settings_.pb) output_fields += " B";
  if (settings_.pjac) output_fields += " jac";
  return output_fields;
}

std::list<double> boris::compose_output_values(double time) const {
  using std::ranges::copy;
  std::list<double> output_values(1, time);
  copy(state_, std::back_inserter(output_values));
  IR3 q = stepper_.get_position(state_);
  if (settings_.pxyz)
    copy((*stepper_.my_morphism())(q), std::back_inserter(output_values));
  if (settings_.pkin) {
    output_values.push_back(stepper_.energy_parallel(state_, time));
    output_values.push_back(stepper_.energy_perpendicular(state_, time));
  }
  if (settings_.pb)
    output_values.push_back(
        field_box_->get_magnetic_field()->magnitude(q, time));
  if (settings_.pjac)
    output_values.push_back(field_box_->get_metric()->jacobian(q));
  return output_values;
}

IR3 boris::get_dot_q(double time) const { return stepper_.get_dot_q(state_); }

IR3 boris::get_q(double time) const { return stepper_.get_position(state_); }

IR3 boris::initial_velocity_from_energy_data() const {
  using gyronimo::aligned_frame;
  using gyronimo::codata::e, gyronimo::codata::m_proton;
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
  settings.pjac = argh_line["pjac"];
  settings.pkin = argh_line["pkin"];
  settings.pxyz = argh_line["pxyz"];
  return settings;
}

double boris::push_state(double time) {
  state_ = stepper_.do_step(state_, time, time_step_);
  return time + time_step_;
}
