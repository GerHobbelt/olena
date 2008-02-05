// Copyright (C) 2008 EPITA Research and Development Laboratory
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

/// \file tests/fun/vv2v/min.cc
/// \brief Test the min functor.

#include <cassert>

#include <mln/metal/vec.hh>
#include <mln/fun/vv2v/min.hh>

int main()
{
  // Scalars.
  mln::fun::vv2v::min<int> min_int;
  assert (min_int(42, 51) == 42);

  // FIXME: We don't have vectors-with-an-order (yet), so this won't
  // work.
#if 0
  // Vectors.
  typedef mln::metal::vec<3, int> vec_t;
  mln::fun::vv2v::min<vec_t> min_vec_t;
  vec_t t, u;
  t.set (1, -2, 3);
  u.set (5,  1, 0);
  assert (min_vec_t(t, u) == t);
#endif
}
