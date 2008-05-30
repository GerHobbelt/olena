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

/*! \file mln/core/concept/doc/site_iterator.hh
 * \brief This file documents the concept of mln::Site_Iterator.
 */

namespace mln
{

  namespace doc
  {

    /*! \brief Documentation class for mln::Site_Iterator.
     *
     * \see mln::Site_Iterator
     */
    template <typename E>
    struct Site_Iterator : public Iterator<E>,
		   public Generalized_Point<E>
    {
      /*! \brief Point_Site associated type.
       * \invariant This type has to derive from mln::Point_Site.
       */
      typedef void psite;

      /*! \brief Convertion into a point-site.
       *
       * \return A point site.
       */
      operator psite() const;
    };

  } // end of namespace mln::doc

} // end of namespace mln
