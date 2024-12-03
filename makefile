# gtrace -- a flexible gyron-tracing application for electromagnetic fields.
# Copyright (C) 2024 Paulo Rodrigues.

# gtrace is free software: you can redistribute it and/or modify it under the
# terms of the GNU General Public License as published by the Free Software
# Foundation, either version 3 of the License, or (at your option) any later
# version.

# gtrace is distributed in the hope that it will be useful, but WITHOUT ANY
# WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
# A PARTICULAR PURPOSE.  See the GNU General Public License for more details.

# You should have received a copy of the GNU General Public License along with
# gtrace. If not, see <https://www.gnu.org/licenses/>.

# @makefile, this file is part of gtrace.

# preliminary tests:
ifeq ($(shell [[ -d gtrace ]] && echo SELF), SELF)
  $(error gtrace: no self-builds, build in a different folder)
endif
ifndef CXX
  $(error gtrace: CXX must point to a valid compiler)
endif
ifndef GYRONIMO_BUILD
  $(error gtrace: GYRONIMO_BUILD must point to a valid build tree)
endif
ifeq ($(shell [[ ! -f $(GYRONIMO_BUILD)/gyronimo-config ]] && echo BAD), BAD)
  $(error gtrace: cannot find a gyronimo build at $(GYRONIMO_BUILD))
endif

# print useful info to stdout:
$(info compiling gtrace: $(CXX) $(CXXFLAGS))
$(info gyronimo build tree: $(GYRONIMO_BUILD))

# craft derivative products:
GTRACE_BUILT_HERE := $(CURDIR)
GTRACE_REPO := $(dir $(realpath $(MAKEFILE_LIST)))
GYRONIMO_INCLUDES := $(shell $(GYRONIMO_BUILD)/gyronimo-config -I)
PARSED_INCLUDES := $(subst :, -I ,$(GYRONIMO_INCLUDES))

# assembles the list of source-code files to be compiled:
BOXES := $(notdir $(wildcard $(GTRACE_REPO)/gtrace/boxes/*.cc))
PREFIXED_BOXES=$(addprefix boxes/, $(BOXES:.cc=.o))
FACTORIES := $(notdir $(wildcard $(GTRACE_REPO)/gtrace/factories/*.cc))
PREFIXED_FACTORIES=$(addprefix factories/, $(FACTORIES:.cc=.o))

# sets separate search paths for headers and sources:
vpath %.hh $(GTRACE_REPO)/gtrace:$(GTRACE_REPO)/gtrace/boxes
vpath %.cc $(GTRACE_REPO)/gtrace

.PHONY: all clean
all: libgtrace.a $(PREFIXED_FACTORIES) main.o gtrace
main.o: main.cc driver_box.hh
libgtrace.a: $(PREFIXED_BOXES)
	@echo '  ->' libgtrace.a
	@ar -r libgtrace.a $(PREFIXED_BOXES);ranlib libgtrace.a
gtrace: $(GTRACE_REPO)/gtrace.in
	@echo '  ->' gtrace
	@sed -e 's GB__ $(GTRACE_BUILT_HERE) g' -e 's GL__ $(CXX) g' \
		-e 's GYB__ $(GYRONIMO_BUILD) g' < $< > $@; chmod +x $@
%.o:
	@echo '  ->' $@
	@$(CXX) -std=c++20 -Wfatal-errors -c $< -o $@ $(CXXFLAGS) \
        -I $(GTRACE_REPO) -I $(PARSED_INCLUDES)

# boxes section:
boxes/boris.o: boxes/boris.cc \
  boris.hh field_box.hh pusher_box.hh | boxes
boxes/driver_box.o: boxes/driver_box.cc driver_box.hh | boxes
boxes/ensemble_async_mpi.o: boxes/ensemble_async_mpi.cc \
  ensemble_async_mpi.hh driver_box.hh observer_box.hh pusher_box.hh | boxes
boxes/littlejohn1983.o: boxes/littlejohn1983.cc \
  littlejohn1983.hh field_box.hh pusher_box.hh | boxes
boxes/q_predicate.o: boxes/q_predicate.cc \
  q_predicate.hh step_printer.hh observer_box.hh | boxes
boxes/single_gyron.o: boxes/single_gyron.cc \
  single_gyron.hh driver_box.hh observer_box.hh pusher_box.hh | boxes
boxes/step_printer.o: boxes/step_printer.cc \
  step_printer.hh observer_box.hh | boxes
boxes/vmec_b.o: boxes/vmec_b.cc vmec_b.hh field_box.hh | boxes

# factories section:
factories/boris.o: factories/boris.cc \
  boris.hh field_box.hh pusher_box.hh | factories
factories/littlejohn1983.o: factories/littlejohn1983.cc \
  littlejohn1983.hh field_box.hh pusher_box.hh | factories
factories/ensemble_async_mpi.o: factories/ensemble_async_mpi.cc \
  ensemble_async_mpi.hh driver_box.hh observer_box.hh pusher_box.hh | factories
factories/q_predicate.o: factories/q_predicate.cc \
  q_predicate.hh step_printer.hh observer_box.hh pusher_box.hh | factories
factories/single_gyron.o: factories/single_gyron.cc \
  single_gyron.hh driver_box.hh observer_box.hh pusher_box.hh | factories
factories/step_printer.o: factories/step_printer.cc \
  step_printer.hh observer_box.hh pusher_box.hh | factories
factories/vmec_b.o: factories/vmec_b.cc vmec_b.hh field_box.hh | factories

# utilities section:
boxes:
	mkdir boxes
factories:
	mkdir factories
clean:
	rm gtrace libgtrace.a main.o; rm -rf boxes factories
