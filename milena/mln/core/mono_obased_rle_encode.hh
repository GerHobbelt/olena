// Copyright (C) 2007 EPITA Research and Development Laboratory
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

#ifndef MLN_CORE_MONO_OBASED_RLE_ENCODE_HH
# define MLN_CORE_MONO_OBASED_RLE_ENCODE_HH

/*! \file mln/core/mono_obased_rle_encode.hh
 *
 * \brief Definintion of function which encodes an image in mono_obased_rle_image.
 */

# include <mln/core/mono_obased_rle_image.hh>

namespace mln
{

  /*!
  ** Encode an image class to a mono_obased_rle_image.
  **
  ** @param input has to respect the Image concept
  ** @param ignore_zero says if zero has to be considered as a valid value
  **
  ** @return mono_obased_rle_image.
  */
  template <typename I>
  mono_obased_rle_image<mln_point(I), mln_value(I)>
  mono_obased_rle_encode(const Image<I>& input, bool ignore_zero = true);

# ifndef MLN_INCLUDE_ONLY
  /*!
  ** test if Point p1 and p2 are on the same line
  */
  template <typename P>
  inline
  bool
  on_the_same_line(const P& p1, const P& p2)
  {
    const unsigned dim = P::dim;
    bool same_line = true;

    for (unsigned n = 0; same_line && n < dim - 1; ++n)
      same_line = (p1[n] == p2[n]);
    return same_line;
  }

  template <typename I>
  inline
  mono_obased_rle_image<mln_point(I), mln_value(I)>
  mono_obased_rle_encode(const Image<I>& input, bool ignore_zero)
  {
    typedef mln_point(I) P;

    const I& ima = exact(input);
    mln_piter(I) p (exact(input).domain());
    unsigned len = 0;
    mln_point(I) rstart;
    mln_value(I) rvalue;
    std::set< mln_value(I) > sv;

    for_all(p)
    {
      if (!ignore_zero || ima(p) != literal::zero)
	sv.insert(ima(p));
    }

    mono_obased_rle_image<mln_point(I), mln_value(I)> output(sv);
    for_all(p)
      if (!ignore_zero || ima(p) != literal::zero || len)
      {
	if (len == 0)
	{
	  ++len;
	  rstart = p;
	  rvalue = ima(p);
	}
	else
	  if (rvalue == ima(p)
	      && on_the_same_line(rstart, mln_point(I)(p)))
	    ++len;
	  else
	  {
	    output.insert(p_run<P>(rstart, len), rvalue);
	    if ((len = (!ignore_zero || ima(p) != literal::zero)))
	    {
	      rstart = p;
	      rvalue = ima(p);
	    }
	  }
      }
    output.finalize();
    return output;
  }

#endif // ! MLN_INCLUDE_ONLY

}

#endif // ! MLN_CORE_MONO_OBASED_RLE_ENCODE_HH
