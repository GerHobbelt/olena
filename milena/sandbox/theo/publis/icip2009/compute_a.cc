// Copyright (C) 2008, 2009 EPITA Research and Development Laboratory
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

/// \file sandbox/theo/publis/icip2009/compute_a.cc

#include <mln/core/image/image2d.hh>
#include <mln/core/image/image_if.hh>
#include <mln/pw/all.hh>
#include <mln/core/alias/neighb2d.hh>
#include <mln/value/int_u8.hh>
#include <mln/io/pgm/load.hh>
#include <mln/io/pgm/save.hh>
#include <mln/debug/println.hh>
#include <mln/core/routine/duplicate.hh>

#include <mln/core/site_set/p_array.hh>
#include <mln/level/sort_psites.hh>
#include <mln/data/fill.hh>
#include <mln/data/paste.hh>
#include <mln/level/compare.hh>

#include <mln/morpho/tree/data.hh>
#include <mln/morpho/tree/compute_attribute_image.hh>
#include <mln/morpho/watershed/flooding.hh>
#include <mln/morpho/elementary/gradient.hh>
#include <mln/labeling/regional_minima.hh>

#include <mln/accu/count.hh>
#include <mln/opt/at.hh>
#include <mln/set/card.hh>



namespace mln
{

  template <typename I>
  void println_par(const I& par)
  {
    int nr = par.nrows(), nc = par.ncols();
    for (int r = 0; r < nr; ++r)
      {
	for (int c = 0; c < nc; ++c)
	  if (opt::at(par, r,c) == point2d(r,c))
	    std::cout << "(    ) ";
	  else
	    std::cout << opt::at(par, r,c) << ' ';
	std::cout << std::endl;
      }
  }


  template <typename P>
  inline
  mln_value(P) find_root__(P& par, const mln_value(P)& x)
  {
    if (par(x) == x)
      return x;
    else
      return par(x) = find_root__(par, par(x));
  }


  template <typename P>
  p_array<P> reverse(const p_array<P>& arr)
  {
    p_array<P> out;
    mln_bkd_piter(p_array<P>) p(arr);
    for_all(p)
      out.insert(p);
    return out;
  }



  //-------------------------------   data



  template <typename I, typename A>
  struct a_data
  {
    typedef mln_site(I) P;
    typedef p_array<P>  S;

    unsigned n_regmins;
    S s;

    mln_ch_value(I, mln_result(A)) a;
    mln_ch_value(I, unsigned)      nchildren;
    mln_ch_value(I, P)             par;
    mln_ch_value(I, bool)          flag;
  };




  //-------------------------------   compute_data



  template <typename I, typename N, typename A>
  a_data<I,A>
    compute_data(const I& f, const N& nbh, A, unsigned& n_regmins,
		 bool echo = false)
  {
    typedef mln_site(I) P;
    typedef p_array<P> S;
    S s = level::sort_psites_increasing(f);
    // s maps increasing attributes.

    mln_ch_value(I, P) par;
    mln_ch_value(I, P) zpar;
    mln_ch_value(I, bool) deja_vu, flag;
    mln_ch_value(I, A) attr;

    n_regmins = f.domain().nsites();


    // Initialization.
    {
      // parent
      initialize(par, f);
      initialize(zpar, f);

      // deja_vu
      initialize(deja_vu, f);
      data::fill(deja_vu, false);

      // flag
      initialize(flag, f);
      data::fill(flag, true);

      // attr
      initialize(attr, f);
    }


    // First Pass.
    {
      P r;
      mln_fwd_piter(S) p(s);
      mln_niter(N) n(nbh, p);
      for_all(p)
      {
	// Make-Set.
	par(p) = p;
	zpar(p) = p;
	attr(p).take_as_init(p);

	for_all(n)
	  if (f.domain().has(n) && deja_vu(n))
	    {
	      r = find_root__(zpar, n);
	      if (r != p)
		{
		  par(r) = p; // Min-tree computation.
		  zpar(r) = p; // Fully compressed union.

		  attr(p).take(attr(r)); // Attribute computation.

		  if (f(r) == f(p))
		    {
		      // Flat zones.
		      if (flag(p) == false && flag(r) == false)
			{
			  // Two non-reg-min components merge (same flat
			  // zone) so we had an extra invalidation.
			  ++n_regmins;
			}
		      else
			flag(p) = flag(p) && flag(r);
		      --n_regmins; // So we get the number of flat zones
			           // minus the non-reg-min flat zones.
		    }
		  else
		    {
		      mln_invariant(f(r) < f(p)); // Increasing browsing.
		      if (flag(p) == true)
			{
			  --n_regmins; // Invalidation.
			  flag(p) = false;
			}
		    }
		}
	    }
	deja_vu(p) = true;
      }

    } // end of First Pass.


    std::cout << "n reg min = " << n_regmins << std::endl;



    // Outputing.

    mln_ch_value(I, mln_result(A)) a; // Attribute image.
    initialize(a, f);
    data::paste(attr, a);

    mln_ch_value(I, unsigned) nchildren; // Number of children.
    initialize(nchildren, f);
    data::fill(nchildren, 0);

    unsigned
      n_sites = 0,
      n_roots = 0,
      n_nodes = 0;


    // Finalization.
    {
      mln_bkd_piter(S) p(s); // Reverse.
      for_all(p)
      {

	// Tree canonization
	P par_p = par(p);
	if (f(par(par_p)) == f(par_p) && par(p) != par(par_p))
	  {
	    par(p) = par(par_p);
	    par_p = par(p);
	  }

	++n_sites; // Counting.

	if (f(par_p) == f(p))
	  {
	    if (par_p == p)
	      {
		++n_roots; // Counting.
		++n_nodes; // Counting.
	      }
	    else
	      {
		a(p) = a(par_p);       // Attribute image.
		flag(p) = flag(par_p); // Flag (regional minima image).
	      }
	  }
	else
	  {
	    ++nchildren(par_p); // Nchildren.
	    ++n_nodes; // Counting.
	  }


// 	// Counting.
// 	++n_sites;
// 	if (par_p == p)
// 	  ++n_roots;
// 	if (par_p == p || f(par_p) != f(p))
// 	  ++n_nodes;

// 	// Attributes.
// 	if (f(par_p) == f(p))
// 	  a(p) = a(par_p);

// 	// Flag (regional minima image).
// 	if (f(par_p) == f(p) && flag(par_p) != flag(p))
// 	  flag(p) = flag(par_p);

// 	// Nchildren.
//   	if (f(par_p) != f(p))
// 	  ++nchildren(par_p);

      }

    }




    std::cout << "%age nodes = "
	      << (float(n_nodes) / n_sites * 100.f) << std::endl
	      << std::endl;

    /*

    // Tests.
    {

      // Tree canonization
      {
	mln_bkd_piter(S) p(s); // Reverse.
	for_all(p)
	{
	  P q = par(p);
	  if (q == p || par(q) == q) // Either p or par(p) is root.
	    continue;
	  if (f(q) == f(p))
	    // p and par(p) within a flat zone
	    // so q=par(p) is the representant
	    mln_invariant(f(par(q)) != f(q));
	}
      }

      // Nchildren and Attributes.
      {
	typedef morpho::tree::data<I,S> T;
 	S s_ = reverse(s);
	T t(f, s_, nbh);

	typedef typename T::function C;
	mln_ch_value(C, unsigned) nc_ref;
	initialize(nc_ref, t.f());
	data::fill(nc_ref, 0);

	mln_fwd_piter(T) p(t.domain());
	for_all(p)
	  if (t.is_a_non_root_node(p))
	    {
	      mln_invariant(t.is_a_node(t.parent(p)));
	      ++nc_ref(t.parent(p));
	    }

	mln_invariant((nchildren | t.domain()) == nc_ref);

	accu::count< util::pix<I> > fixme_;
	mln_ch_value(I, mln_result(A)) a_ref;
	a_ref = morpho::tree::compute_attribute_image(fixme_, t);
	mln_assertion(a == a_ref);
      }

      // Nregmins.
      {
	unsigned n_ref;
	mln_ch_value(I, unsigned) ref;
	ref = labeling::regional_minima(f, nbh, n_ref);
	// debug::println("ref", ref);
	
	mln_assertion(n_regmins == n_ref);
	mln_assertion(((pw::value(ref) != 0) | ref.domain()) == flag);
      }
      {
	unsigned n = 0;
	mln_fwd_piter(S) p(s);
	for_all(p)
	  if (f(par(p)) != f(p) && flag(p))
	    ++n;
	mln_assertion(n == n_regmins);
      }


    } // end of Tests.


    if (echo)
      {
	// Display tree, attributes, flags, and nchildren.

	mln_ch_value(I, char) tree;
	initialize(tree, f);
	data::fill(tree, ' ');
	mln_piter(I) p(f.domain());
	for_all(p)
	  if (par(p) == p) // Root.
	    tree(p) = '@';
	  else
	    if (f(par(p)) != f(p)) // Representative.
	      tree(p) = 'o';
	    else 
	      tree(p) = '.';
	debug::println(tree);

	debug::println("a", a);
	debug::println("flag", flag);
	debug::println("nchildren", nchildren);
      }

*/

    a_data<I,A> dta;
    dta.s = s;
    dta.a = a;
    dta.nchildren = nchildren;
    dta.par = par;
    dta.flag = flag;
    dta.n_regmins = n_regmins;
    
    return dta;

  } // compute_data






  //-------------------------------   compute_filter



  template <typename I, typename A>
  mln_concrete(I)
  compute_filter(const I& f, const a_data<I,A>& dta,
		 unsigned n_objects,
		 mln_result(A)& lambda,
		 bool echo = false)
  {
    typedef mln_site(I) P;
    typedef p_array<P>  S;

    const S& s = dta.s;
    mln_ch_value(I, mln_result(A)) a         = dta.a;
    mln_ch_value(I, unsigned)      nchildren = dta.nchildren;
    mln_ch_value(I, P)             par       = dta.par;
    unsigned                       n_regmins = dta.n_regmins;

    if (n_objects >= n_regmins)
      {
	std::cout << "warning: number of expected objects is greater than number of regmins!" << std::endl;
	std::cout << "aborting..." << std::endl;
	return duplicate(f);
      }
    
    unsigned
      count = dta.n_regmins,
      less  = 0;



    // NOW attributes are sorted increasingly!

    S s_a = level::sort_psites_increasing(a);


    mln_fwd_piter(S) p(s_a);
    for_all(p) // with increasing attribute.
    {
      if (! (par(p) == p || f(par(p)) != f(p))) // Not a node.
	continue;

      if (a(p) < lambda && par(p) != p)
	{
	  mln_assertion(nchildren(par(p)) > 0);
	  --nchildren(par(p));
	  if (nchildren(par(p)) != 0)
	    {
	      if (count <= n_objects)
		{
		  ++less; // minus 1 object wrt the expected number!
		}
	      --count;
	      if (count == n_objects)
		{
		  lambda = a(p) + 1;
		  std::cout << "lambda = " << lambda << std::endl
			    << std::endl;
		}
	    }
	}
    }

    if (less != 0)
      std::cerr << "WARNING: less objects (" << less << ") than expected..." << std::endl
		<< std::endl;

    if (echo)
      debug::println("nchildren =", nchildren);


    // Filtering.
    mln_concrete(I) g;
    initialize(g, f);

    {
      mln_bkd_piter(S) p(s); // Reverse.
      for_all(p)
      {
	if ((par(p) == p || f(par(p)) != f(p)) // p is a node.
	    &&
	    a(p) >= lambda) // Keep it.
	  g(p) = f(p);
	else
	  g(p) = g(par(p)); // Propagate => filtering.
      }


      if (echo)
	debug::println("g =", g);
    }

    return g;

  } // compute_filter


} // mln




void usage(char* argv[])
{
  std::cerr << "usage: " << argv[0] << " input.pgm n_objects filtered.pgm echo" << std::endl;
  std::abort();
}



int main(int argc, char* argv[])
{
  using namespace mln;
  using value::int_u8;

  if (argc != 5)
    usage(argv);

  int n_objects = std::atoi(argv[2]);
  if (n_objects < 0)
    usage(argv);

  int echo = std::atoi(argv[4]);
  if (echo != 0 && echo != 1)
    usage(argv);


  neighb2d nbh = c4();


  typedef image2d<int_u8> I;
  I input;
  io::pgm::load(input, argv[1]);
  if (echo)
    debug::println("input", input);


  I f = morpho::elementary::gradient(input, nbh);
  if (echo)
    debug::println("f (gradient)", f);


  typedef accu::count<point2d> A;
  typedef mln_result_(A) L;

  A area;
  unsigned n_regmins;

  a_data<I,A> dta = compute_data(f, nbh, area, n_regmins, echo);
    
  L lambda;
  I g = compute_filter(f, dta, n_objects, lambda, echo);

  io::pgm::save(g, argv[3]);


  unsigned n_basins;
  morpho::watershed::flooding(g, nbh, n_basins);
  std::cout << "n basins = " << n_basins << std::endl;

}
