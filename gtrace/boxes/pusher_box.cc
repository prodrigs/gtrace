// gtrace -- a flexible gyron-tracing application for electromagnetic fields.
// Copyright (C) 2025 Paulo Rodrigues.

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

// @boxes/pusher_box.cc, this file is part of gtrace.

#include <gtrace/boxes/pusher_box.hh>

pusher_box_t::pusher_box_t(const field_box_t* field_box)
    : field_box_(field_box) {
  if (!field_box) throw std::invalid_argument("empty field_box_t.");
  if (!field_box->is_metric_consistent())
    throw std::runtime_error("inconsistent metrics in field_box_t.");
}
