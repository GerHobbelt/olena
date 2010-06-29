// Copyright (C) 2010 EPITA Research and Development Laboratory (LRDE)
//
// This file is part of Olena.
//
// Olena is free software: you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free
// Software Foundation, version 2 of the License.
//
// Olena is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Olena.  If not, see <http://www.gnu.org/licenses/>.
//
// As a special exception, you may use this file as part of a free
// software project without restriction.  Specifically, if other files
// instantiate templates or use macros or inline functions from this
// file, or you compile this file and link it with other files to produce
// an executable, this file does not by itself cause the resulting
// executable to be covered by the GNU General Public License.  This
// exception does not however invalidate any other reasons why the
// executable file might be covered by the GNU General Public License.

#include <mln/core/image/image2d.hh>
#include <mln/core/alias/neighb2d.hh>
#include <mln/io/pgm/all.hh>

#include <mln/subsampling/antialiased.hh>

#include <mln/value/int_u8.hh>

#include <scribo/debug/usage.hh>


const char *args_desc[][2] =
{
  { "input.pgm", "A gray-scale image." },
  { "ratio", "Scale ratio." },
  {0, 0}
};


int main(int argc, char *argv[])
{
  using namespace mln;

  if (argc != 4)
    return scribo::debug::usage(argv,
				"Subsample.",
				"input.pgm ratio output.pgm",
				args_desc);

  trace::entering("main");

  typedef image2d<value::int_u8> I;
  I input;
  io::pgm::load(input, argv[1]);

  unsigned ratio = atoi(argv[2]);
  io::pgm::save(mln::subsampling::antialiased(input, ratio), argv[3]);

  trace::exiting("main");
}
