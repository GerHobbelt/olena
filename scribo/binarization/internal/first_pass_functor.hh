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

#ifndef SCRIBO_BINARIZATION_INTERNAL_FIRST_PASS_FUNCTOR_HH
# define SCRIBO_BINARIZATION_INTERNAL_FIRST_PASS_FUNCTOR_HH

# include <mln/value/int_u8.hh>

# include <scribo/binarization/sauvola_threshold_image.hh>


// #include <mln/border/adjust.hh>

namespace scribo
{

  namespace binarization
  {

    namespace internal
    {

      using namespace mln;


      unsigned my_find_root(image2d<unsigned>& parent, unsigned x)
      {
	if (parent.element(x) == x)
	  return x;
	return parent.element(x) = my_find_root(parent,
						parent.element(x));
      }


      template <typename I>
      struct first_pass_functor
      {
	const I& input;
	mln_fwd_pixter(const I) pxl;
	double res;
	image2d<unsigned> parent;
	image2d<unsigned> card;
	image2d<bool> msk;
	image2d<value::int_u8> t_sub;

	unsigned n_nbhs;
	util::array<int> dp;

	static const double one_k = 1 - 0.34;
	static const double k_R = 0.34 / 128.0;

	first_pass_functor(const I& input)
	  : input(input),
	    pxl(input)
	{
	  res = 0;
	  pxl.start();

	  initialize(t_sub, input);
	  initialize(parent, input);
	  initialize(msk, input);
	  extension::fill(msk, false);

	  initialize(card, input);
	  data::fill(card, 1);

	  dp = negative_offsets_wrt(input, c4());
	  n_nbhs = dp.nelements();
	}

	void exec(double mean, double stddev)
	{
 	  mln_precondition(pxl.is_valid());

	  unsigned p = pxl.offset();

	  // Use an inlined and developed version of sauvola's
	  // threshold formula.
//  	  value::int_u8 t_p = mean * (one_k + k_R * stddev);

// 	  std::cout << t_p << ", ";

// 	  std::cout << input.element(p) <<  " - " << t_p << std::endl;
          value::int_u8 t_p = sauvola_threshold_formula(mean, stddev);

//  	  std::cout << input.point_at_index(p)
// 		    << " - " << sauvola_threshold_formula(mean, stddev);


	  msk.element(p) = input.element(p) < t_p;
	  t_sub.element(p) = t_p;
	  if (! msk.element(p))
	  {
	    pxl.next();
	    return;
	  }
	  parent.element(p) = p;
	  for (unsigned i = 0; i < n_nbhs; ++i)
	  {
	    unsigned n = p + dp[i];
	    if (! msk.element(n))
	      continue;
	    unsigned r = my_find_root(parent, n);
	    if (r != p)
	    {
	      parent.element(r) = p;
	      card.element(p) += card.element(r);
	    }
	  }

 	  pxl.next(); // next pixel
	}

	void finalize()
	{
 	  mln_assertion(! pxl.is_valid());

// 	  std::cout << std::endl << " ------- " << std::endl;
	}
      };


    } // end of namespace scribo::binarization::internal

  } // end of namespace scribo::binarization

} // end of namespace scribo

#endif // SCRIBO_BINARIZATION_INTERNAL_FIRST_PASS_FUNCTOR_HH
