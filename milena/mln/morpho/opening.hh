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

#ifndef MLN_MORPHO_OPENING_HH
# define MLN_MORPHO_OPENING_HH

/*! \file mln/morpho/opening.hh
 *
 * \brief Morphological opening.
 */

# include <mln/morpho/includes.hh>


namespace mln
{

  namespace morpho
  {

    /*! Morphological opening.
     *
     * This operator is d_{-B} o e_B. 
     */
    template <typename I, typename W>
    mln_concrete(I) opening(const Image<I>& input, const Window<W>& win);


# ifndef MLN_INCLUDE_ONLY

    template <typename I, typename W>
    inline
    mln_concrete(I) opening(const Image<I>& input, const Window<W>& win)
    {
      trace::entering("morpho::opening");
      mln_precondition(exact(input).has_data());
      mln_precondition(! exact(win).is_empty());

      mln_concrete(I) output = dilation(erosion(input, win), win::sym(win));

      // FIXME: Is this postcondition always true, even if the
      // structuring element is not centered?
      mln_postcondition(output <= input);
      trace::exiting("morpho::opening");
      return output;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::morpho

} // end of namespace mln


#endif // ! MLN_MORPHO_OPENING_HH
