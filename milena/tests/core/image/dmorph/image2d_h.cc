// Copyright (C) 2007, 2008, 2009 EPITA Research and Development Laboratory (LRDE)
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

#include <mln/core/image/dmorph/image2d_h.hh>
#include <mln/value/int_u8.hh>
#include <mln/debug/iota.hh>
#include <mln/debug/println.hh>

//FIXME: Write a real test!!

int main()
{
  using namespace mln;
  using value::int_u8;

  image2d_h<int_u8> h(3, 8);

  // FIXME!! The values differs from the test core/image/hexa which
  // may be a bug! debug::iota does not seem to behave correctly.
  debug::iota(h);

  debug::println(h);

//  mln_piter_(image2d_h<int_u8>) p(h.domain());

//  for_all(p)
//    std::cout << p << " - > " << h(p) << std::endl;
}
