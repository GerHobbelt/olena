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

#ifndef MLN_CORE_P_ARRAY_HH
# define MLN_CORE_P_ARRAY_HH

/*! \file mln/core/p_array.hh
 *
 * \brief Definition of a point set class based on std::vector.
 */

# include <vector>

# include <mln/core/internal/point_set_base.hh>
# include <mln/core/internal/pseudo_site_base.hh>
# include <mln/accu/bbox.hh>


namespace mln
{

  // Fwd decls.
  template <typename P> class p_array;
  template <typename P> struct p_array_fwd_piter_;
  template <typename P> struct p_array_bkd_piter_;


  // p_array_psite<P>

  template <typename P>
  class p_array_psite : public internal::pseudo_site_base_< false, // Not mutable.
							    P,
							    p_array_psite<P> >
  {
    typedef p_array_psite<P> self;
    typedef internal::pseudo_site_base_<true, P, self> super;

  public:

    // As a Proxy:

    const P& unproxy() const;

    // As a Site_Proxy:

    typedef typename super::site site;

    const site& to_site() const;

    // As Itself.

    p_array_psite();

    p_array_psite(const p_array<P>& arr, int i);

    int index() const;

    int& index();

    const p_array<P>* target() const;

    void change_target(const p_array<P>& arr);

    void print() const;

  private:

    const p_array<P>* arr_;
    int i_;
  };




  namespace trait
  {

    template <typename P>
    struct site_set_< p_array<P> >
    {
      typedef trait::site_set::nsites::known   nsites;
      typedef trait::site_set::bbox::unknown   bbox;
      typedef trait::site_set::contents::free  contents;
      typedef trait::site_set::arity::multiple arity;
    };

  } // end of namespace trait




  /*! \brief Site set class based on std::vector.
   *
   * This is a multi-set of sites.
   */
  template <typename P>
  class p_array : public internal::site_set_base_< P, p_array<P> >
  {
    typedef internal::site_set_base_< P, p_array<P> > super;
  public:

    /// The associated psite type.
    typedef p_array_psite<P> psite;

    /// The associated site type.
    typedef typename super::site site;

    /// Forward Point_Iterator associated type.
    typedef p_array_fwd_piter_<P> fwd_piter;

    /// Backward Point_Iterator associated type.
    typedef p_array_fwd_piter_<P> bkd_piter; // HOT: FIXME

    /// Constructor.
    p_array();

    /// Constructor from a vector \p vect.
    p_array(const std::vector<P>& vect);

    /// Reserve \p n cells.
    void reserve(std::size_t n);

    /// Test is \p p belongs to this site set.
    bool has(const psite& p) const;

    // FIXME: Add an overload "has(index)".

    /// Give the number of sites.
    std::size_t nsites() const;

    /// Append a point \p p.
    p_array<P>& append(const P& p);

    /// Append an array \p other of points.
    p_array<P>& append(const p_array<P>& other);

    /// Clear this set.
    void clear();

    /// Return the corresponding std::vector of points.
    const std::vector<P>& vect() const;

    /// Return the \p i-th site (constant).
    const P& operator[](unsigned i) const;

    /// Return the \p i-th site (mutable).
    P& operator[](unsigned i);

  protected:

    std::vector<P> vect_;
  };



# ifndef MLN_INCLUDE_ONLY


  // p_array<P>

  template <typename P>
  inline
  p_array<P>::p_array()
  {
  }

  template <typename P>
  inline
  p_array<P>::p_array(const std::vector<P>& vect)
    : vect_(vect)
  {
  }

  template <typename P>
  inline
  void
  p_array<P>::reserve(std::size_t n)
  {
    vect_.reserve(n);
  }

  template <typename P>
  inline
  bool
  p_array<P>::has(const psite& p) const
  {
    mln_precondition(p.target() == this); // FIXME: Refine.
    if (p.index() < 0 || p.index() >= int(vect_.size()))
      return false;
    site s_ = (*this)[p.index()];
    mln_invariant(p.to_site() == s_);
    return true;
  }

  template <typename P>
  inline
  std::size_t
  p_array<P>::nsites() const
  {
    return vect_.size();
  }

  template <typename P>
  inline
  p_array<P>&
  p_array<P>::append(const P& p)
  {
    vect_.push_back(p);
    return *this;
  }

  template <typename P>
  inline
  p_array<P>&
  p_array<P>::append(const p_array<P>& other)
  {
    vect_.insert(vect_.end(),
		 other.vect().begin(), other.vect().end());
    return *this;
  }

  template <typename P>
  inline
  void
  p_array<P>::clear()
  {
    vect_.clear();
  }

  template <typename P>
  inline
  const std::vector<P>&
  p_array<P>::vect() const
  {
    return vect_;
  }

  template <typename P>
  inline
  const P&
  p_array<P>::operator[](unsigned i) const
  {
    mln_precondition(i < nsites());
    return vect_[i];
  }

  template <typename P>
  inline
  P&
  p_array<P>::operator[](unsigned i)
  {
    mln_precondition(i < nsites());
    return vect_[i];
  }


  // p_array_psite<P>

  template <typename P>
  inline
  p_array_psite<P>::p_array_psite()
    : arr_(0),
      i_(0)
  {
  }

  template <typename P>
  inline
  p_array_psite<P>::p_array_psite(const p_array<P>& arr, int i)
    : arr_(&arr),
      i_(i)
  {
  }

  template <typename P>
  inline
  const typename p_array_psite<P>::site&
  p_array_psite<P>::to_site() const
  {
    const site* s;
    internal::get_adr(s, *this);
    return *s;
  }

  template <typename P>
  inline
  int
  p_array_psite<P>::index() const
  {
    return i_;
  }

  template <typename P>
  inline
  int&
  p_array_psite<P>::index()
  {
    return i_;
  }

  template <typename P>
  inline
  const p_array<P>*
  p_array_psite<P>::target() const
  {
    return arr_;
  }

  template <typename P>
  inline
  void
  p_array_psite<P>::change_target(const p_array<P>& arr)
  {
    arr_ = & arr;
  }

  template <typename P>
  inline
  void
  p_array_psite<P>::print() const
  {
    std::cout << i_ << "-th site of " << arr_ << " => site " << to_site() << std::endl;
  }

  template <typename P>
  inline
  const P&
  p_array_psite<P>::unproxy() const
  {
    mln_precondition(arr_ != 0);
    return (*arr_)[i_];
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


# include <mln/core/p_array_piter.hh>


#endif // ! MLN_CORE_P_ARRAY_HH
