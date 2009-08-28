// Copyright (C) 2009 EPITA Research and Development Laboratory (LRDE)
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

#include <mln/io/ppm/load.hh>
#include <mln/io/pbm/save.hh>

#include <scribo/binarization/sauvola.hh>
#include <scribo/debug/usage.hh>

const char *args_desc[][2] =
{
  { "input.ppm", "A color image." },
  {0, 0}
};


int main(int argc, char *argv[])
{
  using namespace mln;
  using value::rgb8;

  if (argc != 3)
    return scribo::debug::usage(argv,
				"Binarization of a color image based on Sauvola's algorithm.",
				"input.ppm output.pbm",
				args_desc, "A binary image.");

  trace::entering("main");

  image2d<rgb8> input;
  io::ppm::load(input, argv[1]);


  io::pbm::save(scribo::binarization::sauvola(input),
		argv[2]);


  trace::exiting("main");
}
