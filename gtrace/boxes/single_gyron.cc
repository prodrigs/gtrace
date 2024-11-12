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

// @boxes/single_gyron.cc, this file is part of gtrace.

#include <gtrace/boxes/single_gyron.hh>

#include <iostream>
#include <memory>

int single_gyron::operator()(int argc, char* argv[]) const {
  auto argh_line = this->argh_line();
  if (argh_line[{"h", "help"}]) {
    this->print_help();
    return 1;
  }

  this->print_header(argc, argv);
  std::unique_ptr<pusher_box_t> pusher {create_linked_pusher_box(argh_line)};
  std::unique_ptr<observer_box_t> observer {
      create_linked_observer_box(argh_line)};

  double time_final;
  argh_line("tfinal", 1) >> time_final;
  this->integrate_orbit(pusher.get(), observer.get(), time_final);
  return 0;
}

void single_gyron::print_help() {
  driver_box_t::print_version();
  std::string usage_message =
      "\n"
      "driver -> gtrace::single_gyron\n"
      "Usage: gtrace link_options -- [-tfinal=] [box_options]\n"
      "\n"
      "Integrates a single gyron (ie, particle, guiding centre, etc) as\n"
      "defined by the state type of the invoked pusher_box.\n"
      "Options:\n"
      "  -tfinal=  Time-integration limit (default 1, in pusher_box units).\n";
  std::cout << usage_message << std::endl;
  driver_box_t::print_help();

  auto help_argh = argh::parser("-help");
  pusher_box_t* dummy_pusher = create_linked_pusher_box(help_argh);
  if (dummy_pusher) delete dummy_pusher;
  observer_box_t* dummy_observer = create_linked_observer_box(help_argh);
  if (dummy_observer) delete dummy_observer;
}
