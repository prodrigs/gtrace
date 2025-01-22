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

// @boxes/driver_box.cc, this file is part of gtrace.

#include <gyronimo/version.hh>

#include <gtrace/boxes/driver_box.hh>

#include <chrono>
#include <sstream>

driver_box_t::driver_box_t(int argc, char* argv[])
    : argh_line_(argh::parser(argc, argv)) {}

std::string driver_box_t::header_string(int argc, char* argv[]) {
  std::ostringstream header;
  header << "# gtrace -- a flexible gyron-tracing application "
         << "for electromagnetic fields.\n"
         << "# powered by ::gyronimo::v" << gyronimo::version_major << "."
         << gyronimo::version_minor << "." << gyronimo::version_patch
         << " (git-commit:" << gyronimo::git_commit_hash << ").\n";
  header << "# args: ";
  for (int i = 1; i < argc; i++) header << argv[i] << " ";
  return header.str();
}

std::string driver_box_t::integrate_orbit(
    pusher_box_t* pusher, const observer_box_t* observer, double tfinal) const {
  auto tick_0 = std::chrono::steady_clock::now();
  double time = 0;
  while ((*observer)(pusher, time) && time <= tfinal)
    time = pusher->push_state(time);
  auto tick_1 = std::chrono::steady_clock::now();
  if (argh_line_["peek-beyond-tfinal"]) (*observer)(pusher, time);
  std::ostringstream elapsed_time_line;
  elapsed_time_line << "# elapsed time: "
                    << std::chrono::duration<double>(tick_1 - tick_0);
  return elapsed_time_line.str();
}
