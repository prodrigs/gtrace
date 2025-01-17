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

// @boxes/ensemble_async_mpi.hh, this file is part of gtrace.

#ifndef GTRACE_ENSEMBLE_ASYNC_MPI
#define GTRACE_ENSEMBLE_ASYNC_MPI

#include <gtrace/boxes/driver_box.hh>

#include <fstream>
#include <iostream>

/*!
Parallel (MPI) integration of a gyron ensemble.
-----------------------------------------------

Sets up the `pusher_box_t`, `field_box_t`, and `observer_box_t` objects required
to follow an ensemble of gyrons (ie, particle, guiding centre, etc). The actual
type of gyron to be traced is defined by the state type of the invoked
`pusher_box_t`.

The ensemble is split by a number of fully independent mpi parallel processes,
each one integrating asynchronously its corresponding sub-ensemble. Each gyron
within a given sub-ensemble is created by reading the options from a line in the
corresponding input file (each one named as `prefix-nnn` with `nnn` the process
rank number, as many input files as running processes or sub-ensembles). To save
disk space, these input files may contain only the options particular to each
distinct gyron, with common (or shared) options being supplied via the invoking
command line. For each sub-ensemble, the output of the invoked puser and
observer boxes is collected into a file named `prefix-nnn.cout`. No other
processing is performed. To ensure that no MPI communications are needed, the
pusher, field, and observer boxes are replicated by each process.

Driver options:

 + `-prefix=name` Prefix of input & output filenames.
 + `-tfinal=val` Time-integration limit (default 1, in `pusher_box_t` units).
!*/
class ensemble_async_mpi : public driver_box_t {
 public:
  template<class StreamType> class stream_redirector;
  ensemble_async_mpi(int argc, char* argv[]);
  virtual ~ensemble_async_mpi();
  virtual int operator()(int argc, char* argv[]) const;
 private:
  int mpi_rank_, mpi_size_;
  std::string convert_argv_to_string(char* argv[]) const;
  std::pair<std::ifstream, std::string> get_input_stream(
      const argh::parser& argh_line, int mpi_rank) const;
};

template<class StreamType> class ensemble_async_mpi::stream_redirector {
 public:
  stream_redirector(StreamType& passive, const std::string& filename);
  ~stream_redirector();
 private:
  std::ofstream active_stream_;
  StreamType& passive_stream_;
  std::streambuf* stored_passive_buffer_ = nullptr;
};

template<class StreamType>
ensemble_async_mpi::stream_redirector<StreamType>::stream_redirector(
    StreamType& passive, const std::string& filename)
    : passive_stream_(passive) {
  active_stream_.open(filename);
  if (!active_stream_.is_open()) {
    std::cerr << "gtrace::ensemble_async_mpi: cannot open file \"" << filename
              << "\" for writing." << std::endl;
    std::exit(1);
  }
  stored_passive_buffer_ = passive_stream_.rdbuf();
  passive_stream_.rdbuf(active_stream_.rdbuf());
}

template<class StreamType>
ensemble_async_mpi::stream_redirector<StreamType>::~stream_redirector() {
  active_stream_.flush();
  active_stream_.close();
  passive_stream_.rdbuf(stored_passive_buffer_);
}

#endif  // GTRACE_ENSEMBLE_ASYNC_MPI
