// Copyright (C) 2006 EPITA Research and Development Laboratory
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

#include <iostream>

#include <xtd/math.hh>


int main()
{
  using xtd::_1;
  using xtd::_2;
  using xtd::lit;

  (_1 + _2)(_1, _1);

  (_1 + _2)(_1, lit(1.f));

  ;

  std::cout << (_1 + _2) .bind_2(2.6f) (2.5f) << std::endl;

//   std::cout << (_1 + _2)(_1, 1.)(5.) << std::endl;

  // autre probl�me :
  // std::cout << (_1 + 1)(5.f) << std::endl;

}
