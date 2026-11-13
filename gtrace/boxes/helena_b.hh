// gtrace -- a flexible gyron-tracing application for electromagnetic fields.
// Copyright (C) 2024-2026 Paulo Rodrigues and Leonor Roque.

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

// @boxes/helena_b.hh, this file is part of gtrace.

#ifndef GTRACE_HELENA_B
#define GTRACE_HELENA_B

#include <gyronimo/fields/equilibrium_helena.hh>
#include <gyronimo/interpolators/bicubic_gsl.hh>

#include <gtrace/boxes/field_box.hh>

using gyronimo::bicubic_gsl_factory;
using gyronimo::equilibrium_helena;
using gyronimo::metric_helena;
using gyronimo::morphism_helena;
using gyronimo::parser_helena;

/*!
Magnetic field by the mhd-equilibrium code HELENA.

Sets a `gyronimo::equilibrium_helena` object (and its dependencies) by reading a
hmap file produced by the mhd-equilibrium code HELENA (G. Huysmans et al., Int.
J. Mod. Phys. C **2**, 371 (1991).

## General options:

`-cached`
  : Builds a level-1 cached version of the objects `gyronimo::{equilibrium_helena,
    metric_helena, morphism_helena}`. Eventual performance improvements depend
    heavily on how particular pushers call this field and cannot be assumed a
    priori.

`-helena-file=val`
  : Path to the hmap file produced by HELENA.
!*/
class helena_b : public field_box_t {
 public:
  helena_b() = delete;
  helena_b(const argh::parser& arghs);
  virtual ~helena_b() {};
  virtual const IR3field* get_electric_field() const override {
    return nullptr;
  };
  virtual const IR3field* get_magnetic_field() const override;
  virtual const metric_covariant* get_metric() const override;
 private:
  std::unique_ptr<bicubic_gsl_factory> ifactory_;
  std::unique_ptr<parser_helena> parser_;
  std::unique_ptr<morphism_helena> morphism_;
  std::unique_ptr<metric_helena> metric_;
  std::unique_ptr<equilibrium_helena> magnetic_field_;
};

#endif  // GTRACE_HELENA_B
