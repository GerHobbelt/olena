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

#ifndef OLN_VALUE_BUILTIN_HH
# define OLN_VALUE_BUILTIN_HH

# include <oln/core/concept/value.hh>


namespace oln
{


  // Fwd decl.
  namespace internal { template <typename Exact> struct builtin_base; }


  // Virtual types.
  template <typename Exact>
  struct vtypes< internal::builtin_base<Exact> >
  {
    typedef stc::final< stc::is<Value> > category;
  };


  namespace internal
  {

    // Base class for builtin types.
    template <typename Exact>
    struct builtin_base : public Value<Exact>
    {
    protected:
      builtin_base();
    };


# ifndef OLN_INCLUDE_ONLY

    template <typename Exact>
    builtin_base<Exact>::builtin_base()
    {
    }

# endif // ! OLN_INCLUDE_ONLY
  
  } // end of namespace oln::internal



  // unsigned, int, float, double,...

  template <>
  struct super_trait_< unsigned >
  {
    typedef internal::builtin_base<unsigned> ret;
  };

  template <>
  struct super_trait_< int >
  {
    typedef internal::builtin_base<int> ret;
  };

  template <>
  struct super_trait_< float >
  {
    typedef internal::builtin_base<float> ret;
  };

  template <>
  struct super_trait_< double >
  {
    typedef internal::builtin_base<double> ret;
  };



} // end of namespace oln


// FIXME: Bad?
# include <oln/value/builtin_traits.hh>


#endif // ! OLN_VALUE_BUILTIN_HH

