// Copyright (C) 2007, 2008 EPITA Research and Development Laboratory
// (LRDE)
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

#ifndef MLN_LITERAL_ALL_HH
# define MLN_LITERAL_ALL_HH

/// \file mln/literal/all.hh
///
/// File that includes all literals.

// Technical note:
//
// With some recent g++ versions, we have a warning due to literal
// conversions (namely: "type qualifiers ignored on function return
// type").  This warning comes from the "operator T()" method
// instantiated with T being a const type, for instance, when
// literal::zero is given to a procedure expecting a "const int&"
// object.   This warning will be avoided in very recent versions
// of C++ thanks to the new compiling option -Wignore_qualifiers.


namespace mln
{

  /// Namespace of literals.
  namespace literal {}

}


# include <mln/literal/zero.hh>
# include <mln/literal/one.hh>

# include <mln/literal/origin.hh>

# include <mln/literal/white.hh>
# include <mln/literal/black.hh>

# include <mln/literal/grays.hh>
# include <mln/literal/colors.hh>

# include <mln/literal/ops.hh>


#endif // ! MLN_LITERAL_ALL_HH
