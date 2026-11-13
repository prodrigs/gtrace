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

// @boxes/helena_b.cc, this file is part of gtrace.

#include <gyronimo/fields/IR3field_c1_cache.hh>
#include <gyronimo/metrics/metric_cache.hh>
#include <gyronimo/metrics/morphism_cache.hh>

#include <gtrace/boxes/helena_b.hh>

const IR3field* helena_b::get_magnetic_field() const {
  return magnetic_field_.get();
}
const metric_covariant* helena_b::get_metric() const { return metric_.get(); }

helena_b::helena_b(const argh::parser& arghs) : ifactory_(nullptr) {
  std::string helena_filename;
  arghs("helena-file", "") >> helena_filename;
  parser_ = std::make_unique<parser_helena>(helena_filename);
  ifactory_ = std::make_unique<bicubic_gsl_factory> (false, (parser_->is_symmetric() ? 0 : 9), (parser_->is_symmetric() ? 9 : 0));
  if (arghs["cached"]) {
    using gyronimo::IR3field_c1_cache;
    using gyronimo::metric_cache, gyronimo::morphism_cache;
    morphism_ = std::make_unique<morphism_cache<morphism_helena>>(
        parser_.get(), ifactory_.get());
    metric_ = std::make_unique<metric_cache<metric_helena>>(morphism_.get(), ifactory_.get());
    magnetic_field_ = std::make_unique<IR3field_c1_cache<equilibrium_helena>>(
        metric_.get(), ifactory_.get());
  } else {
    morphism_ = std::make_unique<morphism_helena>(
        parser_.get(), ifactory_.get());
    metric_ = std::make_unique<metric_helena>(morphism_.get(), ifactory_.get());
    magnetic_field_ =
        std::make_unique<equilibrium_helena>(metric_.get(), ifactory_.get());
  }
}
