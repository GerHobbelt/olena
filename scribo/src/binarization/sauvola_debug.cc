// Copyright (C) 2009, 2010 EPITA Research and Development Laboratory
// (LRDE)
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

#include <mln/io/magick/load.hh>
#include <mln/io/pbm/save.hh>
#include <mln/io/pgm/save.hh>

#include <mln/data/stretch.hh>
#include <mln/data/convert.hh>
#include <mln/data/saturate.hh>

#include <scribo/binarization/local_threshold.hh>
#include <scribo/binarization/sauvola_threshold_image_debug.hh>
#include <scribo/debug/usage.hh>

const char *args_desc[][2] =
{
  { "input.*", "An image." },
  { "output.pbm", "A binary image." },
  { "mean.pgm", "The local mean image." },
  { "stddev.pgm", "The local standard deviation image." },
  { "mean_factor", "Mean value factor (default 1)." },
  { "stddev_factor", "Standard deviation value factor (default 10)." },
  { "w", "Window size (default 51)." },
  { "k", "Sauvola's formulae parameter (default 0.34)." },
  {0, 0}
};


int main(int argc, char *argv[])
{
  using namespace mln;
  using namespace scribo::binarization;

  if (argc < 5 || argc >= 9)
    return scribo::debug::usage(argv,
				"Binarization based on Sauvola's algorithm.",
				"input.* output.pbm mean.pgm stddev.pgm <mean_factor> <stddev_factor> <w> <k>",
				args_desc);

  trace::entering("main");

  unsigned w;
  if (argc >= 8)
    w = atoi(argv[7]);
  else
    w = 51;

  double k;
  if (argc >= 9)
    k = atof(argv[8]);
  else
    k = 0.34f;

  std::cout << "Using w=" << w << " and k=" << k << std::endl;

  if (argc >= 6)
    scribo::binarization::internal::mean_debug_factor   = atoi(argv[5]);
  if (argc >= 7)
    scribo::binarization::internal::stddev_debug_factor = atoi(argv[6]);

  image2d<value::rgb8> input;
  io::magick::load(input, argv[1]);


  image2d<float> mean, stddev, thres;
  initialize(mean, input);
  initialize(stddev, input);
  initialize(thres, input);

  image2d<value::int_u8>
    gima = data::transform(input,
			   mln::fun::v2v::rgb_to_int_u<8>());


  image2d<bool>
    out = local_threshold(gima,
			  sauvola_threshold_image_debug(gima, w, k,
							mean, stddev, thres));


  io::pbm::save(out, argv[2]);

  io::pgm::save(data::stretch(value::int_u8(), mean), argv[3]);
  io::pgm::save(data::stretch(value::int_u8(), stddev), argv[4]);

  io::pgm::save(data::saturate(value::int_u8(), mean), "mean_saturated.pgm");
  io::pgm::save(data::saturate(value::int_u8(), stddev), "stddev_saturated.pgm");

  io::pgm::save(data::saturate(value::int_u8(), thres), "thres_saturated.pgm");

  trace::exiting("main");
}
