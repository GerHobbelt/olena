// Copyright (C) 2007, 2008 EPITA Research and Development Laboratory
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

#ifndef MLN_CORE_VALUE_ENC_IMAGE_CC_
# define MLN_CORE_VALUE_ENC_IMAGE_CC_

/*! \file mln/core/image/value_enc_image.hh
 *
 * \brief Define the class value_enc_image.
 */

# include <mln/core/internal/image_primary.hh>

# include <mln/core/pset_array.hh>
# include <mln/core/pset_array_psite.hh>
# include <mln/core/p_runs.hh>

# include <vector>
# include <mln/util/tracked_ptr.hh>

namespace mln
{

  // Fwd decl.
  template <typename P, typename T> struct value_enc_image;


  namespace internal
  {

    /// \internal Data structure for \c mln::value_enc_image<P,T>.
    template <typename P, typename T>
    struct data< value_enc_image<P, T> >
    {
      data();

      /// Image values.
      std::vector<T> values_;

      /// domain of the image
      pset_array< p_runs_<P> > domain_;
    };

  } // end of namespace mln::internal


  namespace trait
  {

    template <typename P, typename T>
    struct image_< value_enc_image<P,T> > :
      default_image_< T, value_enc_image<P,T> >
    {
      typedef trait::image::category::primary category;

      typedef trait::image::access::browsing   access;

      // FIXME: Put the right dimension.
      typedef trait::image::space::two_d     space;

      typedef trait::image::size::regular    size;
      typedef trait::image::support::aligned support;

      typedef trait::image::border::none     border;
      typedef trait::image::data::linear     data;
      typedef trait::image::io::read_only    io;
      typedef trait::image::speed::slow      speed;
    };

  } // end of namespace mln::trait


  /*! \brief Value encoded image.
   *  Associate a value to set of runs.
   */
  template <typename P, typename T>
  class value_enc_image :
    public internal::image_primary< pset_array< p_runs_<P> >,
				     value_enc_image<P, T> >
  {
  public:

    /// Value related typedefs
    typedef T value;
    typedef T& lvalue;
    typedef const T rvalue;

    /// Domain related typedefs
    typedef pset_array_psite< runs_psite<P> > psite;
    typedef pset_array< p_runs_<P> > pset;

    /// Skeleton.
    typedef value_enc_image< tag::psite_<P>, tag::value_<T> > skeleton;

    value_enc_image();

    /// Add a new set of ranges  to the image.
    void insert(const p_runs_<P>& ps, T value);

    /// Add a new range to the image
    void insert(const p_run<P>& pr, T value);

    /// Tell if the image has the given point site.
    bool has(const psite& ps) const;

    /// Read-only access to the image value located at the site \site.
    rvalue operator() (const psite& site) const;

    /// Read-write access to the image value located at the site \site.
    lvalue operator() (const psite& site);

    /// Test if this image has been initialized.
    bool has_data() const;

    /// Give the definition domain.
    const pset& domain() const;
  };


# ifndef MLN_INCLUDE_ONLY

  namespace internal
  {

    // internal::data< value_enc_image<P, T> >

    template <typename P, typename T>
    inline
    data< value_enc_image<P,T> >::data()
    {
    }

  } // end of namespace mln::internal


  template <typename P, typename T>
  inline
  value_enc_image<P, T>::value_enc_image()
  {
    this->data_ = new internal::data< value_enc_image<P,T> >();
  }

  template <typename P, typename T>
  inline
  bool
  value_enc_image<P, T>::has_data() const
  {
    return this->data_->values_.size() != 0;
  }

  template <typename P, typename T>
  inline
  void
  value_enc_image<P, T>::insert(const p_runs_<P>& ps, T value)
  {
    if (!this->has_data())
      this->data_ = new internal::data< value_enc_image<P,T> >();

    this->data_->domain_.insert(ps);
    this->data_->values_.push_back(value);
  }

  template <typename P, typename T>
  inline
  void
  value_enc_image<P, T>::insert(const p_run<P>& pr, T value)
  {
    unsigned i = 0;
    typename std::vector<T>::iterator it;

    for (it  = this->data_->values_.begin();
	 it != this->data_->values_.end() && *it != value;
	 ++it)
      ++i;

    if (it != this->data_->values_.end())
      this->data_->domain_[i].insert(pr);
    else
    {
      p_runs_<P> ps = p_runs_<P> ();
      ps.insert(pr);
      this->data_->domain_.insert(ps);
      this->data_->values_.push_back(value);
    }
  }

  template <typename P, typename T>
  inline
  bool
  value_enc_image<P, T>::has(const typename value_enc_image<P, T>::psite& site)
    const
  {
    return this->data_->domain_.has(site);
  }

  template <typename P, typename T>
  inline
  typename value_enc_image<P, T>::rvalue
  value_enc_image<P, T>::operator() (const typename
				     value_enc_image<P, T>::psite& site)
    const
  {
    mln_precondition(this->has(site));

    return this->data_->values_[site.to_index()];
  }

  template <typename P, typename T>
  inline
  typename value_enc_image<P, T>::lvalue
  value_enc_image<P, T>::operator() (const typename
				     value_enc_image<P, T>::psite& site)
  {
    mln_precondition(this->has(site));

    return this->data_->values_[site.to_index()];
  }

  template <typename P, typename T>
  inline
  const typename value_enc_image<P, T>::pset&
  value_enc_image<P, T>::domain() const
  {
    return this->data_->domain_;
  }

# endif // ! MLN_INCLUDE_ONLY


} // end of namespace mln


#endif // ! MLN_CORE_VALUE_ENC_IMAGE_CC_

