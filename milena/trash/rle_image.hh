// Copyright (C) 2007, 2008 EPITA Research and Development Laboratory
// (LRDE)
//
// This file is part of the Milena Library.  This library is free
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

#ifndef MLN_CORE_IMAGE_RLE_IMAGE_HH
# define MLN_CORE_IMAGE_RLE_IMAGE_HH

/// \file mln/core/image/rle_image.hh
///
/// Definition of an image with rle encoding.

# include <mln/core/internal/run_image.hh>
# include <mln/core/site_set/p_set_of.hh>
# include <mln/core/site_set/p_run.hh>
# include <mln/value/set.hh>
# include <vector>

namespace mln
{

  // Forward declaration
  template <typename P, typename T> struct rle_image;


  namespace internal
  {

    /// Data structure for \c mln::rle_image<P,T>.
    template <typename P, typename T>
    struct data< rle_image<P,T> >
    {
      data();

      /// Image values.
      std::vector<T> values_;

      /// domain of the image
      p_set_of< p_run<P> > domain_;

      /// Return the size of the data in memory.
      unsigned memory_size() const;

      /// Finalize the domain (internal use).
      void finalize();
    };

  } // end of namespace mln::internal


  namespace trait
  {

    template <typename P, typename T>
    struct image_< rle_image<P,T> > : default_image_< T, rle_image<P,T> >
    {
      typedef trait::image::category::primary category;

      // FIXME: update with new properties.
//      typedef trait::image::access::browsing   access;
      // FIXME: Put the right dimension.
//      typedef trait::image::space::two_d     space;
//      typedef trait::image::size::regular    size;
//      typedef trait::image::support::aligned support;
//
//      typedef trait::image::border::none     border;
//      typedef trait::image::data::linear     data;
//      typedef trait::image::io::read_only    io;
//      typedef trait::image::speed::slow      speed;
    };

  } // end of namespace mln::trait


  /*! RLE image.
   *
   *
   * Parameter \c P is the type of the image points.
   * Parameter \c T is the type of the pixel values.
   * This image is not point wise accessible.
   */
  template <typename P, typename T>
  class rle_image : public internal::run_image_< T, P, rle_image<P, T> >
  {
  public:
    typedef T value;
    typedef T& lvalue;
    typedef const T rvalue;
    typedef p_set_of< p_run<P> > pset;
    typedef mln_psite(pset) psite;


    /// Skeleton.
    typedef rle_image< tag::psite_<P>, tag::value_<T> > skeleton;

    /// Constructor.
    rle_image();

    /// Add a new range to the image.
    void insert(const p_run<P>& pr, T value);

    /// Read-only access to the image value located at point \p p.
    rvalue operator() (const mln_psite(pset)& site) const;

    /// Read-write access to the image value located at point \p p.
    lvalue operator() (const mln_psite(pset)& site);

    /// Test if this image has been initialized.
    bool is_valid() const;

    /// Give the definition domain.
    const pset& domain() const;

  };


# ifndef MLN_INCLUDE_ONLY

  namespace internal
  {

    // internal::data< rle_image<I,S> >

    template <typename P, typename T>
    inline
    data< rle_image<P,T> >::data()
    {
    }

    template <typename P, typename T>
    inline
    unsigned
    data< rle_image<P,T> >::memory_size() const
    {
      return sizeof(T) * values_.size() + domain_.memory_size();
    }

    template <typename P, typename T>
    inline
    void
    data< rle_image<P,T> >::finalize()
    {
      domain_.finalize();
    }

  } // end of namespace mln::internal

  template <typename P, typename T>
  inline
  rle_image<P, T>::rle_image()
  {
    this->data_ = new internal::data< rle_image<P,T> >();
  }

  template <typename P, typename T>
  inline
  bool
  rle_image<P, T>::is_valid() const
  {
    return this->data_->values_.size() != 0;
  }

  template <typename P, typename T>
  inline
  void
  rle_image<P, T>::insert(const p_run<P>& pr, T value)
  {
    if (!this->is_valid())
      this->data_ = new internal::data< rle_image<P,T> >();
    mln_assertion(this->data_->values_.size() == 0 ||
		  pr[0] > this->data_->domain_[this->data_->domain_.nsites() - 1].start());
    this->data_->domain_.insert(pr);
    this->data_->values_.push_back(value);
  }

  template <typename P, typename T>
  inline
  typename rle_image<P, T>::rvalue
  rle_image<P, T>::operator() (const mln_psite(pset)& site)
    const
  {
    mln_precondition(this->has(site));
    return this->data_->values_[site.index()];
  }

  template <typename P, typename T>
  inline
  typename rle_image<P, T>::lvalue
  rle_image<P, T>::operator() (const mln_psite(pset)& site)
  {
    mln_precondition(this->has(site));
    return this->data_->values_[site.index()];
  }

  template <typename P, typename T>
  inline
  const typename rle_image<P, T>::pset&
  rle_image<P, T>::domain() const
  {
    return this->data_->domain_;
  }

# endif // ! MLN_INCLUDE_ONLY


} // end of namespace mln


#endif // ! MLN_CORE_IMAGE_RLE_IMAGE_HH
