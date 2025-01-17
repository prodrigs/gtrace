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

// @boxes/vmec_b.cc, this file is part of gtrace.

#include <gyronimo/fields/IR3field_c1_cache.hh>
#include <gyronimo/metrics/metric_cache.hh>
#include <gyronimo/metrics/morphism_cache.hh>

#include <gtrace/boxes/vmec_b.hh>

const IR3field* vmec_b::get_magnetic_field() const {
  return magnetic_field_.get();
}
const metric_covariant* vmec_b::get_metric() const { return metric_.get(); }

vmec_b::vmec_b(const argh::parser& argh_line)
    : ifactory_(new cubic_gsl_factory()) {
  parser_ = std::make_unique<parser_vmec>(argh_line[1]);

  gyronimo::multiroot_c1::settings_t settings = {
      .method = gsl_multiroot_fdfsolver_newton,
      .is_residual_tested = (argh_line["test-residue"] ? true : false)};
  argh_line("iterations", 10) >> settings.iterations;
  argh_line("abstol", 1e-12) >> settings.tolerance_abs;
  argh_line("reltol", 1e-12) >> settings.tolerance_rel;

  if (argh_line["cached"]) {
    using gyronimo::IR3field_c1_cache;
    using gyronimo::metric_cache, gyronimo::morphism_cache;
    morphism_ = std::make_unique<morphism_cache<morphism_vmec>>(
        parser_.get(), ifactory_.get(), settings);
    metric_ = std::make_unique<metric_cache<metric_vmec>>(morphism_.get());
    magnetic_field_ = std::make_unique<IR3field_c1_cache<equilibrium_vmec>>(
        metric_.get(), ifactory_.get());
  } else {
    morphism_ = std::make_unique<morphism_vmec>(
        parser_.get(), ifactory_.get(), settings);
    metric_ = std::make_unique<metric_vmec>(morphism_.get());
    magnetic_field_ =
        std::make_unique<equilibrium_vmec>(metric_.get(), ifactory_.get());
  }
}
