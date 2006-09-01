// Copyright (C) 2001, 2002, 2003, 2004, 2006 EPITA Research and
// Development Laboratory
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

#ifndef OLENA_CORE_ABSTRACT_DPOINT_ND_HH
# define OLENA_CORE_ABSTRACT_DPOINT_ND_HH

# include <mlc/value.hh>
# include <xtd/vec.hh>
# include <oln/core/abstract/dpoint.hh>


namespace oln
{

  // Forward declaration.
  namespace abstract { template <typename E> class dpoint_nd; }


  // Super type declaration.
  template <typename E>
  struct set_super_type< abstract::dpoint_nd<E> >
  {
    typedef abstract::dpoint<E> ret;
  };


  namespace abstract
  {

    template <typename E>
    class dpoint_nd : public abstract::dpoint<E>
    {
      typedef dpoint_nd<E> self_t;
      typedef oln_type_of(E, dim) dim;
      typedef oln_type_of(E, coord) coord_t;

    public:

      enum { n = mlc_value(dim) };

      bool impl_equal(const self_t& rhs) const
      {
	return v_ == rhs.v_;
      }
      
      coord_t operator[](unsigned i) const
      {
	assert(i < n);
	return v_[i];
      }
      
      coord_t& operator[](unsigned i)
      {
	assert(i < n);
	return v_[i];
      }

      const xtd::vec<n,coord_t>& vec() const
      {
	return v_;
      }

    protected:

      /// Ctor.
      dpoint_nd()
      {}

      /// Ctor.
      dpoint_nd(const xtd::vec<n,coord_t>& v) :
	v_(v)
      {}

      xtd::vec<n,coord_t> v_;
    };

  } // end of namespace oln::abstract


} // end of namespace oln


#endif // ! OLENA_CORE_ABSTRACT_DPOINT_ND_HH
