// Copyright (C) 2007, 2008 EPITA Research and Development Laboratory (LRDE)
//
// This file is part of the Olena Library.  This library is free
// software; you can redistribute it and/or modify it under the terms
// of the GNU General Public License version 2 as published by the
// Free Software Foundation.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this library; see the file COPYING.  If not, write to
// the Free Software Foundation, 51 Franklin Street, Fifth Floor,
// Boston, MA 02111-1307, USA.
//
// As a special exception, you may use this file as part of a free
// software library without restriction.  Specifically, if other files
// instantiate templates or use macros or inline functions from this
// file, or you compile this file and link it with other files to
// produce an executable, this file does not by itself cause the
// resulting executable to be covered by the GNU General Public
// License.  This exception does not however invalidate any other
// reasons why the executable file might be covered by the GNU General
// Public License.

/*! \file tests/display/save_and_show.cc
 *
 * \brief Tests on mln::display::save_and_show.
 */

#include <mln/core/image/image2d.hh>
#include <mln/io/pbm/load.hh>
#include <mln/make/win_chamfer.hh>
#include <mln/geom/chamfer.hh>
#include <mln/value/rgb8.hh>
#include <mln/core/image/sub_image.hh>
#include <mln/core/image_if_value.hh>

#include <mln/core/alias/w_window2d_int.hh>
#include <mln/display/show.hh>
#include <mln/io/ppm/save.hh>
#include <mln/display/save_and_show.hh>
#include <mln/level/fill.hh>

#include "tests/data.hh"


int main()
{
  using namespace mln;

  unsigned max = 51;

  image2d<bool> input = io::pbm::load(MLN_IMG_DIR "/toto.pbm");

  // Create a weighted windows :
  // 0 2 0
  // 2 p 2
  // 0 2 0
  const w_window2d_int& w_win = make::mk_chamfer_3x3_int<2, 0> ();

  // Call chamfer for a distance image.
  image2d<unsigned> tmp = geom::chamfer(input, w_win, max);

  // Call color_pretty for sub_image.
  for (unsigned i = 2; i < 6; i += 2)
    {
      image_if_value<image2d<unsigned> > t = tmp | i;
      display::save_and_show (t, "display", 1);
    }

  /// Test with color image.
  {
    image2d<value::rgb8> ima (100, 100);
    level::fill(ima, value::rgb8 (0, 0, 255));
    display::save_and_show (ima, "display", 1);
  }

}
