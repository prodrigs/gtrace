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

// @tools/odeint_wrapper.hh, this file is part of gtrace.

#ifndef GTRACE_ODEINT_WRAPPER
#define GTRACE_ODEINT_WRAPPER

#include <gyronimo/dynamics/odeint_adapter.hh>

#include <gtrace/tools/odeint_stepper.hh>

#include <boost/numeric/odeint/stepper/adams_bashforth_moulton.hpp>
#include <boost/numeric/odeint/stepper/runge_kutta4.hpp>
#include <boost/numeric/odeint/stepper/runge_kutta_fehlberg78.hpp>

template<typename ConcreteStepper, typename EqSystem>
class stepper_wrapper : public odeint_stepper<EqSystem> {
 public:
  using state_t = EqSystem::state;
  virtual ~stepper_wrapper() {};
  virtual double do_step(
      const EqSystem& eqs, state_t& state, double t, double dt) override {
    stepper_.do_step(gyronimo::odeint_adapter(&eqs), state, t, dt);
    return t + dt;
  };
 private:
  ConcreteStepper stepper_;
};

template<typename EqSystem>
odeint_stepper<EqSystem>* odeint_stepper_factory(
    const std::string& stepper_name) {
  using namespace boost::numeric::odeint;
  using state_t = EqSystem::state;
  if (stepper_name == "adams")
    return new stepper_wrapper<adams_bashforth_moulton<8, state_t>, EqSystem>;
  if (stepper_name == "fehlberg")
    return new stepper_wrapper<runge_kutta_fehlberg78<state_t>, EqSystem>;
  if (stepper_name == "rungekutta")
    return new stepper_wrapper<runge_kutta4<state_t>, EqSystem>;
  std::cerr << "gtrace::odeint_stepper_factory: unsupported stepper "
            << stepper_name << ".\n";
  std::exit(1);
  return nullptr;
}

#endif  // GTRACE_ODEINT_WRAPPER
