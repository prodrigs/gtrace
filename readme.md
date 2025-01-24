gtrace (powered by ::gyronimo::)
================================

*- A flexible application for gyron-tracing in electromagnetic fields -*

"... there are more particle-tracing applications in heaven and earth,
Horatio, than are dreamt of in your personal software package..."

gtrace is a highly flexible command-line application to trace the orbits
of gyrons (ie, gyro-moving beings like charged particles, guiding
centres, etc) in electromagnetic fields. Unlike most tracing
applications, which are usually hardwired to particular pusher methods
or field descriptions, gtrace is able to arbitrarily combine different
field descriptions (via field boxes), gyron types and pusher algorithms
(via pusher boxes), and common simulation settings (via driver boxes).

Build instructions:
-------------------

The build process may be tailored by setting three environment
variables: the c++ compiler to use (must support -std=c++20) in CXX, any
compilation flags in CXXFLAGS, and the path to a working build tree of
the `gyronimo` library in GYRONIMO_BUILD. The standard three-step
procedure is as follows:

1. Change into a clean build folder (no self-builds allowed);
2. Run `make -f /path/to/my/repository/gtrace/makefile`;
3. There's no step 3;

At this stage, the build folder may be kept as is, may be moved to
somewhere else, or referred to via suitable symlinks - just pick your
favourite choice. Most users will interact with the package only via the
executable script `gtrace` (installed at the root of the build tree) and
the object-code files under the folder `factories/`.

Usage examples:
---------------

General help about the gtrace script is found by running `gtrace -h`.
More information about specific drivers, pushers, fields, and observers
boxes can be found in the commented source code.

In the example below, the Boris algorithm is used to push a single
charged particle (ie., the gyron native to boris.o) within the
vmec-produced magnetic field (as defined by vmec_b.o) that is stored in
the file wout_my_nice_vmec_field.nc. The observer step_printer.o will be
invoked at every time step (5000 of them, spanning 1us), printing some
information about the corresponding Boris state (eg, the cartesian
position requested by the flag -pxyz), which is redirected to the file
output.txt.

```
gtrace -d single_gyron.o -p boris.o -f vmec_b.o -o step_printer.o -- \
    -lref=1 -vref=1e6 -mass=4 -charge=2 -qu=0.5 -qv=0 -qw=0 \
    -energy=3.5e6 -pitch=0.6 -tfinal=1 -samples=5000 -pxyz \
    -vmec-file=wout_my_nice_vmec_field.nc > output.txt
```

Users frowning about long and cluttered command lines may find it more
convenient to have the messy list of options neatly stored in text files
and then expand their content when executing, as in

```
gtrace $(< my_modules.txt) -- $(< omy_ptions.txt)
```
