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

// @boxes/littlejohn1983.cc, this file is part of gtrace.

#include <gyronimo/core/codata.hh>
#include <gyronimo/metrics/metric_connected.hh>

#include <gtrace/boxes/littlejohn1983.hh>
#include <gtrace/tools/odeint_wrapper.hh>

#include <algorithm>

std::string littlejohn1983::compose_output_fields() const {
  std::string output_fields("# fields: t qu qv qw vpar");
  if (settings_.pxyz) output_fields += " x y z";
  if (settings_.pkin) output_fields += " Epar Eperp";
  if (settings_.pb) output_fields += " B";
  if (settings_.pjac) output_fields += " jac";
  return output_fields;
}

std::list<double> littlejohn1983::compose_output_values(double time) const {
  using std::ranges::copy;
  std::list<double> output_values(1, time);
  copy(state_, std::back_inserter(output_values));
  IR3 q = eqs_motion_.get_position(state_);
  if (settings_.pxyz) {
    using gyronimo::metric_connected, gyronimo::morphism;
    auto g = static_cast<const metric_connected*>(field_box_->get_metric());
    const morphism& morph(*g->my_morphism());
    copy(morph(q), std::back_inserter(output_values));
  }
  if (settings_.pkin) {
    output_values.push_back(eqs_motion_.energy_parallel(state_));
    output_values.push_back(eqs_motion_.energy_perpendicular(state_, time));
  }
  if (settings_.pb)
    output_values.push_back(
        field_box_->get_magnetic_field()->magnitude(q, time));
  if (settings_.pjac)
    output_values.push_back(field_box_->get_metric()->jacobian(q));
  return output_values;
}

IR3 littlejohn1983::get_dot_q(double time) const {
  auto ds = eqs_motion_(state_, time);
  return {ds[0], ds[1], ds[2]};
};

double littlejohn1983::get_energy_tilde(const settings_t& s) {
  using gyronimo::codata::e, gyronimo::codata::m_proton;
  const double energy_ref_ev = 0.5 * m_proton * s.mass * s.vref * s.vref / e;
  return s.energy / energy_ref_ev;
}

double littlejohn1983::get_mu_tilde(
    const settings_t& s, const field_box_t* field_box) {
  IR3 q_initial = {s.qu, s.qv, s.qw};
  double B = field_box->get_magnetic_field()->magnitude(q_initial, 0);
  return (1 - s.pitch * s.pitch) * get_energy_tilde(s) / B;
}

IR3 littlejohn1983::get_q(double time) const {
  return eqs_motion_.get_position(state_);
};

bool littlejohn1983::is_pxyz_inconsistent(
    const settings_t& s, const field_box_t* fb) {
  using gyronimo::metric_connected;
  return s.pxyz && !(dynamic_cast<const metric_connected*>(fb->get_metric()));
}

littlejohn1983::littlejohn1983(const settings_t& s, const field_box_t* fb)
    : pusher_box_t(fb), settings_(s), time_step_(s.time_final / s.samples),
      stepper_(odeint_stepper_factory<guiding_centre>(s.odeint)),
      eqs_motion_(
          s.lref, s.vref, s.charge / s.mass, get_mu_tilde(s, fb),
          dynamic_cast<const gyronimo::IR3field_c1*>(fb->get_magnetic_field()),
          fb->get_electric_field()) {
  if (is_pxyz_inconsistent(s, field_box_))
    throw std::runtime_error("inconsistent -pxyz and non-connected metric.");
  IR3 q_initial = {s.qu, s.qv, s.qw};
  state_ = eqs_motion_.generate_state(
      q_initial, get_energy_tilde(s),
      (s.pitch < 0 ? guiding_centre::minus : guiding_centre::plus), 0);
}

littlejohn1983::settings_t littlejohn1983::parse_settings(
    const argh::parser& arghs) {
  settings_t settings;
  arghs("samples", 512) >> settings.samples;
  arghs("tfinal", 1) >> settings.time_final;
  arghs("lref", 1) >> settings.lref;
  arghs("vref", 1) >> settings.vref;
  arghs("mass", 1) >> settings.mass;
  arghs("charge", 1) >> settings.charge;
  arghs("qu", 0.1) >> settings.qu;
  arghs("qv", 0) >> settings.qv;
  arghs("qw", 0) >> settings.qw;
  arghs("energy", 1) >> settings.energy;
  arghs("pitch", 0.5) >> settings.pitch;
  arghs("gyrophase", 0) >> settings.gyrophase;
  arghs("odeint", "rungekutta") >> settings.odeint;
  settings.pb = arghs["pb"];
  settings.pjac = arghs["pjac"];
  settings.pkin = arghs["pkin"];
  settings.pxyz = arghs["pxyz"];
  return settings;
}
