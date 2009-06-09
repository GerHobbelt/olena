// Copyright (C) 2007, 2008 EPITA Research and Development Laboratory (LRDE)
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

#include <mln/core/alias/w_window3d_int.hh>
#include <mln/win/cube3d.hh>

#include <mln/convert/to.hh>
#include <mln/convert/to_image.hh>
#include <mln/convert/to_fun.hh>
#include <mln/estim/sum.hh>


int f(mln::point3d p)
{
  return p.sli() + p.row() + p.col();
}


int main()
{
  using namespace mln;

  {
    int ws[27] = {-3, -2, -3,
		  -2,  1, -2,
		  -3, -2, -3,
		   2,  0, -2,
		   0,  0,  0,
		   2,  0, -2,
		   3,  2,  3,
		   2, -1,  2,
		   3,  2,  3};
    w_window3d_int w_win = make::w_window3d(ws);

    image3d<int> ima = convert::to_image(w_win);
    w_window3d_int w_win_2 = convert::to<w_window3d_int>(ima);
    mln_assertion(w_win_2 == w_win);
  }

  {
    w_window3d_int w_win = make::w_window(win::cube3d(3),
					  convert::to_fun(f));
    //  -3 -2 -1  0 +1
    //  -2 -1  0 +1 +2
    //  -1  0 +1 +2 +3
    image3d<int> ima = convert::to_image(w_win);
    mln_assertion(estim::sum(ima) == 0);
  }

}
