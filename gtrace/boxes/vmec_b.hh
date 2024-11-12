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

// @boxes/vmec_b.hh, this file is part of gtrace.

#ifndef GTRACE_VMEC_B
#define GTRACE_VMEC_B

#include <gyronimo/fields/equilibrium_vmec.hh>
#include <gyronimo/interpolators/cubic_gsl.hh>

#include <gtrace/boxes/field_box.hh>

using gyronimo::cubic_gsl_factory;
using gyronimo::equilibrium_vmec;
using gyronimo::metric_vmec;
using gyronimo::morphism_vmec;
using gyronimo::parser_vmec;

class vmec_b : public field_box_t {
 public:
  static void print_help();
  vmec_b(const argh::parser& argh_line);
  virtual ~vmec_b() {};
  virtual const IR3field* get_electric_field() const override {
    return nullptr;
  };
  virtual const IR3field* get_magnetic_field() const override {
    return magnetic_field_.get();
  };
 private:
  std::unique_ptr<cubic_gsl_factory> ifactory_;
  std::unique_ptr<parser_vmec> parser_;
  std::unique_ptr<morphism_vmec> morphism_;
  std::unique_ptr<metric_vmec> metric_;
  std::unique_ptr<equilibrium_vmec> magnetic_field_;
};

#endif  // GTRACE_VMEC_B
