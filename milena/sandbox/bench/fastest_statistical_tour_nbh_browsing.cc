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

#include <mln/core/image/image2d.hh>
#include <mln/core/alias/neighb2d.hh>
#include <mln/pw/all.hh>
#include <mln/data/compare.hh>
#include <mln/debug/println.hh>
#include <mln/util/timer.hh>



# define loop(n) for (unsigned i = 0; i < n; ++i)



namespace mln
{


  // test_

  template <typename I>
  void test_(const std::string& routine, float t, const I& ima, unsigned n)
  {
    dpoint2d dp(1, 1);
    box2d
      b = ima.domain(),
      inner(b.pmin() + dp, b.pmax() - dp);
    std::cout << routine << ' ' << t << std::endl;
    if ((ima | inner) != (pw::cst(4 * n) | inner))
      std::cerr << "bug in " << routine << '!' << std::endl;
    // FIXME: other tests...
  }



  // piter

  template <typename I, typename N>
  void piter_based(I& ima, const N& nbh)
  {
    mln_piter(I) p(ima.domain());
    mln_niter(N) n(nbh, p);
    for_all(p)
      if ((p.row() + p.col()) % 2 == 0)
	for_all(n)
	  ++ima(n);
    for_all(p)
      if ((p.row() + p.col()) % 2 == 1)
	for_all(n)
	  ++ima(n);
  }


  // row_col

  template <typename I, typename N>
  void row_col_based(I& ima, const N& nbh)
  {
    const unsigned nrows = ima.nrows(), ncols = ima.ncols();
    unsigned n_nbhs = nbh.size();
    for (int row = 0; row < nrows; ++row)
      for (int col = row % 2; col < ncols; col += 2)
	for (unsigned j = 0; j < n_nbhs; ++j)
	  ++ima.at_(row + nbh.dp(j).row(), col + nbh.dp(j).col());
    for (int row = 0; row < nrows; ++row)
      for (int col = ! (row % 2); col < ncols; col += 2)
	for (unsigned j = 0; j < n_nbhs; ++j)
	  ++ima.at_(row + nbh.dp(j).row(), col + nbh.dp(j).col());
  }


  // run_ptr

  template <typename I, typename N>
  void run_ptr_based(I& ima, const N& nbh)
  {
    mln_box_runstart_piter(I) s(ima.domain());
    const unsigned n = s.run_length();

    util::array<int> dp = offsets_wrt(ima, nbh);
    const unsigned n_nbhs = dp.nelements();

    // half-tour 0
    unsigned skip = 0;
    for_all(s)
    {
      mln_value(I)* ptr = &ima(s) + skip;
      for (unsigned i = 0; i < n; i += 2)
	{
	  for (unsigned j = 0; j < n_nbhs; ++j)
	    ++*(ptr + dp(j));
	  ptr += 2;
	}
      skip = ! skip;
    }

    // half-tour 1
    skip = 1;
    for_all(s)
    {
      mln_value(I)* ptr = &ima(s) + skip;
      for (unsigned i = 0; i < n; i += 2)
	{
	  for (unsigned j = 0; j < n_nbhs; ++j)
	    ++*(ptr + dp(j));
	  ptr += 2;
	}
      skip = ! skip;
    }
  }


} // end of namespace mln





int main()
{
  using namespace mln;

  const unsigned n = 10, size = 1000;
  image2d<char> ima(size, size);
  neighb2d nbh = c4();
  util::timer t;
  std::string routine;


  // piter

  {
    routine = "piter";
    data::fill(ima, 0);
    t.start();
    loop(n) piter_based(ima, nbh);
    test_(routine, t.stop(), ima, n);
  }


  // row_col

  {
    routine = "row_col";
    data::fill(ima, 0);
    t.start();
    loop(n) row_col_based(ima, nbh);
    test_(routine, t.stop(), ima, n);
  }


  // run_ptr

  {
    routine = "run_ptr";
    data::fill(ima, 0);
    t.start();
    loop(n) run_ptr_based(ima, nbh);
    test_(routine, t.stop(), ima, n);
  }

}
