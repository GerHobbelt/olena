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

#ifndef MLN_CORE_PVEC_HH
# define MLN_CORE_PVEC_HH

/*! \file mln/core/pvec.hh
 *
 * \brief Definition of a point set class based on std::vector.
 */

# include <vector>

# include <mln/core/concept/point_set.hh>
# include <mln/accu/bbox.hh>


namespace mln
{

  // Fwd decls.
  template <typename P> struct pvec_fwd_piter_;
  template <typename P> struct pvec_bkd_piter_;


  /*! \brief Point set class based on std::vector.
   *
   * This is a multi-set of points.
   *
   * \warning We have some troubles with point set comparison based on
   * a call to npoints().  FIXME: Explain!
   *
   * \todo Make it work with P being a Point_Site.
   */
  template <typename P>
  class pvec : public Point_Set< pvec<P> >
  {
  public:

    /// Point associated type.
    typedef P point;

    /// Point_Site associated type.
    typedef P psite;

    /// Forward Point_Iterator associated type.
    typedef pvec_fwd_piter_<P> fwd_piter;

    /// Backward Point_Iterator associated type.
    typedef pvec_bkd_piter_<P> bkd_piter;

    /// Constructor.
    pvec();

    /// Constructor from a vector \p vect.
    pvec(const std::vector<P>& vect);

    /// Test is \p p belongs to this point set.
    bool has(const P& p) const;

    /// Give the number of points.
    std::size_t npoints() const;

    /// Give the exact bounding box.
    const box_<P>& bbox() const;

    /// Append a point \p p.
    pvec<P>& append(const P& p);

    /// Clear this set.
    void clear();

    /// Return the corresponding std::vector of points.
    const std::vector<P>& vect() const;

    /// Return the \p i-th point.
    const P& operator[](unsigned i) const;

  protected:

    std::vector<P> vect_;
    mutable accu::bbox<P> bb_;
    mutable bool bb_needs_update_;

    void update_bb_() const;
    // FIXME: Add invariant  bb_.is_valid() <=> npoints() != 0
  };



# ifndef MLN_INCLUDE_ONLY

  template <typename P>
  pvec<P>::pvec()
  {
    bb_needs_update_ = false;
  }

  template <typename P>
  pvec<P>::pvec(const std::vector<P>& vect)
    : vect_(vect)
  {
    bb_needs_update_ = true;
  }

  template <typename P>
  void
  pvec<P>::update_bb_() const
  {
    bb_.init();
    for (unsigned i = 0; i < vect_.size(); ++i)
      bb_.take(vect_[i]);
    bb_needs_update_ = false;
  }

  template <typename P>
  bool
  pvec<P>::has(const P& p) const
  {
    for (unsigned i = 0; i < vect_.size(); ++i)
      if (vect_[i] == p)
	return true;
    return false;
  }

  template <typename P>
  std::size_t
  pvec<P>::npoints() const
  {
    return vect_.size();
  }

  template <typename P>
  const box_<P>&
  pvec<P>::bbox() const
  {
    mln_precondition(npoints() != 0);
    if (bb_needs_update_)
      update_bb_();
    return bb_.to_value();
  }

  template <typename P>
  pvec<P>&
  pvec<P>::append(const P& p)
  {
    vect_.push_back(p);
    if (! bb_needs_update_)
      bb_needs_update_ = true;
    return *this;
  }

  template <typename P>
  void
  pvec<P>::clear()
  {
    vect_.clear();
    bb_needs_update_ = false;
  }

  template <typename P>
  const std::vector<P>&
  pvec<P>::vect() const
  {
    return vect_;
  }

  template <typename P>
  const P&
  pvec<P>::operator[](unsigned i) const
  {
    mln_precondition(i < npoints());
    return vect_[i];
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


# include <mln/core/pvec_piter.hh>


#endif // ! MLN_CORE_PVEC_HH
