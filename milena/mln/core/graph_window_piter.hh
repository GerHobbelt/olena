// Copyright (C) 2007, 2008 EPITA Research and Development Laboratory (LRDE)
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

#ifndef MLN_CORE_GRAPH_WINDOW_PITER_HH
# define MLN_CORE_GRAPH_WINDOW_PITER_HH

/// \file   mln/core/graph_window_piter.hh
/// \brief  Definition of a point iterator on a graph window.

/* FIXME: Factor those classes:

   - mln::graph_window_fwd_piter
   - mln::graph_neighborhood_fwd_piter
   - mln::line_graph_window_fwd_piter
   - mln::line_graph_neighborhood_fwd_piter.

   and later (when they get implemented):

   - mln::graph_window_bkd_piter
   - mln::graph_neighborhood_bkd_piter
   - mln::line_graph_window_bkd_piter
   - mln::line_graph_neighborhood_bkd_piter.  */

# include <mln/core/concept/point_iterator.hh>
# include <mln/core/p_graph.hh>
# include <mln/core/graph_psite.hh>

/* FIXME: Doc.  */

/* FIXME: Due to the poor interface of mln::p_graph and
   mln::util::graph, we show to much implementation details here.
   Enrich their interfaces to avoid that.  */

namespace mln
{
  // Fwd decls.
  template <typename P> class p_graph;
  template <typename P> class graph_psite;


  /*----------------------------.
  | graph_window_fwd_piter<P>.  |
  `----------------------------*/

  template <typename P>
  class graph_window_fwd_piter :
    public Point_Iterator< graph_window_fwd_piter<P> > // or Iterator<...>?
  {
    typedef graph_window_fwd_piter<P> self_;
    typedef Point_Iterator< self_ > super_;

  public:
    typedef graph_psite<P> psite;

    enum { dim = P::dim };
    // FIXME: Dummy value.
    typedef void mesh;

    typedef P point;
    // FIXME: Dummy typedef.
    typedef void dpoint;
    typedef mln_coord(P) coord;

  public:
    template <typename W, typename Pref>
    graph_window_fwd_piter(const W& win, const Point_Site<Pref>& p_ref);

    bool is_valid() const;
    void invalidate();
    void start();

    void next_();
    bool adjacent_or_equal_to_p_ref_() const;
    /// Update the internal data of the iterator.
    void update_();

    const point& to_point() const;

    const psite& to_psite() const;

    operator point() const;

    operator psite() const;

    /// Return the \a i th coordinate of the (iterated) point.
    coord operator[](unsigned i) const;

  private:
    /// The ``central'' psite of the window.
    const psite& p_ref_;
    /// An internal iterator on the set of nodes of the underlying graph.
    util::node_id id_;
    /// The psite corresponding to this iterator.
    psite psite_;
    /// The point corresponding to this iterator.
    point p_;
  };

  /*----------------------------.
  | graph_window_bkd_piter<P>.  |
  `----------------------------*/


# ifndef MLN_INCLUDE_ONLY

  // FIXME: Currently, argument win is ignored.
  template <typename P>
  template <typename W, typename Pref>
  inline
  graph_window_fwd_piter<P>::graph_window_fwd_piter(const W& /* win */,
						    const Point_Site<Pref>& p_ref)
    : p_ref_(exact(p_ref).to_psite()),
      // Initialize psite_ to a dummy value.
      psite_(p_ref_.pg(), p_ref_.pg().npoints()),
      p_()
  {
    // Invalidate id_.
    invalidate();
  }

  template <typename P>
  inline
  bool
  graph_window_fwd_piter<P>::is_valid() const
  {
    // FIXME: We depend too much on the implementation of util::graph
    // here.  The util::graph should provide the service to abstract
    // these manipulations.
    return id_ < p_ref_.pg().gr_.nnodes();
  }

  template <typename P>
  inline
  void
  graph_window_fwd_piter<P>::invalidate()
  {
    id_ = p_ref_.pg().gr_.nnodes();
  }

  template <typename P>
  inline
  void
  graph_window_fwd_piter<P>::start()
  {
    id_ = 0;
    if (!adjacent_or_equal_to_p_ref_())
      next_();
    /* FIXME: This is redundant with the end of next_(), but we might
       change the implementation of start_() when we'll fix it later,
       and no longer use next_().  */
    if (is_valid())
      update_();
  }

  template <typename P>
  inline
  void
  graph_window_fwd_piter<P>::next_()
  {
    /* FIXME: This is inefficient.  The graph structure should be able
       to produce the set of adjacent nodes fast.  Boost Graphs
       probably provides adequates structures to fetch these
       neighbors in constant time.  */
    /* FIXME: Moreover, the behavior of next shall depend on the
       window, which is not the case now!  (Currently, next_() behaves
       as win was always an elementary window.) */
    do
      ++id_;
    while (is_valid() && !adjacent_or_equal_to_p_ref_());
    if (is_valid())
      update_();
  }

  template <typename P>
  inline
  bool
  graph_window_fwd_piter<P>::adjacent_or_equal_to_p_ref_() const
  {
    // FIXME: Likewise, this is inefficient.

    // Check wether the iterator points to P_REF_.
    if (id_ == p_ref_.id())
      return true;

    // Check whether the iterator is among the neighbors of P_REF_.
    {
      // Paranoid assertion.
      assert (p_ref_.id() < p_ref_.pg().gr_.nnodes());
      // FIXME: This is too low-level.  Yet another service the graph
      // should provide.
      typedef std::vector<util::node_id> adjacency_type;
      const adjacency_type& p_ref_neighbs =
	p_ref_.pg().gr_.nodes()[p_ref_.id()]->edges;
      adjacency_type::const_iterator j =
	std::find (p_ref_neighbs.begin(), p_ref_neighbs.end(), id_);
      if (j != p_ref_neighbs.end())
	return true;
    }

    // Otherwise, the iterator is not adjacent to P_REF_.
    return false;
  }

  template <typename P>
  inline
  void
  graph_window_fwd_piter<P>::update_()
  {
    // Update psite_.
    psite_ = graph_psite<P>(p_ref_.pg(), id_);
    // Update p_.
    p_ = p_ref_.pg().gr_.node_data(id_);
  }

  template <typename P>
  inline
  const P&
  graph_window_fwd_piter<P>::to_point() const
  {
    return p_;
  }

  template <typename P>
  inline
  const graph_psite<P>&
  graph_window_fwd_piter<P>::to_psite() const
  {
    return psite_;
  }

  template <typename P>
  inline
  graph_window_fwd_piter<P>::operator P() const
  {
    mln_precondition(is_valid());
    return p_;
  }

  template <typename P>
  inline
  graph_window_fwd_piter<P>::operator graph_psite<P>() const
  {
    mln_precondition(is_valid());
    return psite_;
  }

  template <typename P>
  inline
  mln_coord(P)
  graph_window_fwd_piter<P>::operator[](unsigned i) const
  {
    assert(i < dim);
    return p_[i];
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln

#endif // ! MLN_CORE_GRAPH_WINDOW_PITER_HH
