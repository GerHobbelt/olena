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

/*! \file tests/core/mono_obased_rle_image.cc
 *
 * \brief Test on mln::mono_obased_rle_image.hh.
 */

#include <mln/core/image/image2d.hh>
#include <mln/value/int_u8.hh>
#include <mln/io/pgm/load.hh>
#include <mln/pw/all.hh>
#include <mln/core/neighb2d.hh>

#include <mln/labeling/blobs.hh>
#include <mln/level/transform.hh>
#include <mln/level/paste.hh>
#include <mln/level/compare.hh>
#include <mln/io/pgm/save.hh>

#include <mln/core/image/mono_obased_rle_encode.hh>

#include "tests/data.hh"


struct fold_t : public mln::Function_v2v< fold_t >
{
  typedef mln::value::int_u8 result;
  result operator()(unsigned i) const { return i == 0 ? 0 : (i - 1) % 255 + 1; }
};


int main()
{
  using namespace mln;
  using value::int_u8;

  image2d<int_u8> lena;
  io::pgm::load(lena, MLN_IMG_DIR "/small.pgm");
  image2d<int_u8> cmp(lena.domain());

  unsigned n;
  image2d<unsigned> labels =
    labeling::blobs((pw::value(lena) > pw::cst(172u)) | lena.domain(),
		    c4(), n);

  mono_obased_rle_image<point2d, int_u8> rle =
    mono_obased_rle_encode(level::transform(labels, fold_t()));

  std::cout << n << ", compression ratio: " << rle.compression() << std::endl;

  level::fill(cmp, literal::zero);
  level::paste(rle, cmp);

  mln_assertion(cmp == level::transform(labels, fold_t()));
}
