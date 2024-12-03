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

// @boxes/vmec_b.cc, this file is part of gtrace.

#include <gyronimo/fields/IR3field_c1_cache.hh>
#include <gyronimo/metrics/metric_cache.hh>
#include <gyronimo/metrics/morphism_cache.hh>

#include <gtrace/boxes/vmec_b.hh>

const IR3field* vmec_b::get_magnetic_field() const {
  return magnetic_field_.get();
}
const metric_covariant* vmec_b::get_metric() const { return metric_.get(); }

void vmec_b::print_help() {
  std::string help_message =
      "field_box -> gtrace::vmec_b\n"
      "Usage: gtrace link_options -- [...] [vmec_b options] netcdf_file\n"
      "\n"
      "Sets a gyronimo::equilibrium_vmec object (and its dependencies) by\n"
      "reading the vmec produced netcdf_file (mandatory argument).\n"
      "Options:\n"
      "  -cached   Builds a level-1 cached version of the objects\n"
      "            gyronimo::{equilibrium_vmec, metric_vmec, morphism_vmec}.\n"
      "            It may improve performance, but not necessarily so.\n"
      "Other options (actually the gyronimo::morphism_vmec settings):\n"
      "  -abstol=, -reltol=\n"
      "            Absolute and relative tolerances (default 1e-12).\n"
      "  -iterations=\n"
      "            Number of iterations (default 10).\n"
      "  -test-residue\n"
      "            Tests residual value, not the solution delta (default).\n";
  std::cout << help_message << std::endl;
}

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
