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

#ifndef SCRIBO_EXTRACT_PRIMITIVE_LINES_PATTERN_HH
# define SCRIBO_EXTRACT_PRIMITIVE_LINES_PATTERN_HH

/// \file
///
/// Extract lines matching a specific pattern.

# include <mln/core/concept/image.hh>
# include <mln/core/alias/window2d.hh>
# include <mln/core/routine/duplicate.hh>

# include <mln/accu/transform_line.hh>
# include <mln/accu/count_value.hh>

# include <mln/data/paste.hh>


namespace scribo
{

  namespace extract
  {

    namespace primitive
    {

      using namespace mln;


      /// Extract lines with a specific pattern
      ///
      /// \param[in] input_ A binary image.
      /// \param[in] length The minimum line length.
      /// \param[in] dir    The direction of the lines.
      /// \param[in] win_   A window corresponding to the line pattern.
      ///
      /// \return A image with lines of direction \p dir.
      //
      template <typename I, typename W>
      mln_concrete(I)
      lines_pattern(const Image<I>& input_, unsigned length,
		    unsigned dir, const Window<W>& win_);


# ifndef MLN_INCLUDE_ONLY

      template <typename I, typename W>
      mln_concrete(I)
      lines_pattern(const Image<I>& input_, unsigned length,
		    unsigned dir, const Window<W>& win_)
      {
	trace::entering("mln::lines_pattern");

	const I& input = exact(input_);
	const W& win = exact(win_);
	mlc_is(mln_value(I), bool)::check();
	mln_precondition(input.is_valid());

	accu::count_value<bool> accu(true);
	mln_ch_value(I,unsigned)
	  tmp = accu::transform_line(accu, input, length, dir);

	mln_concrete(I) output;
	initialize(output, input);

	mln_piter(I) p(input.domain());
	mln_qiter(window2d) q(win, p);
	for_all(p)
	{
	  if (length - tmp(p) > (0.2f * length))
	  {
	    output(p) = false;
	    continue;
	  }

	  unsigned bg_count = 0;
	  for_all(q)
	    bg_count += tmp(q);

	  output(p) = (2 * length - bg_count) > (2 * length * 0.90f);
	}


	trace::exiting("mln::lines_pattern");
	return output;
      }

# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace mln::impl::generic

  } // end of namespace mln::impl

} // end of namespace mln


#endif // ! SCRIBO_EXTRACT_PRIMITIVE_LINES_PATTERN_HH
