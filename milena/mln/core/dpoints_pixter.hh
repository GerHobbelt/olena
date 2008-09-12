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

#ifndef MLN_CORE_DPOINTS_PIXTER_HH
# define MLN_CORE_DPOINTS_PIXTER_HH

/// \file mln/core/dpoints_pixter.hh
/// 
/// \brief Definition of forward and backward mln::dpoint-based
/// iterators for pixels iterations.

# include <cassert>
# include <vector>

# include <mln/core/concept/pixel_iterator.hh>
# include <mln/core/internal/pixel_impl.hh>


namespace mln
{

  /*------------------------.
  | dpoints_fwd_pixter<I>.  |
  `------------------------*/

  /// \brief A generic forward iterator on the pixels of a
  /// dpoint-based window or neighborhood.
  /// 
  /// Parameter \c I is the image type.
  template <typename I>
  class dpoints_fwd_pixter
    : public Pixel_Iterator< dpoints_fwd_pixter<I> >,
      public internal::pixel_impl_< I, dpoints_fwd_pixter<I> >
  {
    typedef typename internal::pixel_impl_< I, dpoints_fwd_pixter<I> > super_;

  public:
    /// \brief Constructor (using an image).
    /// 
    /// \param[in] image The image to iterate over.
    /// \param[in] dps   An object (neighborhood or window) that can
    ///                  provide a set of delta-points.
    /// \param[in] p_ref Center (resp. reference) point of the
    ///                  neighborhood (resp. window).
    template <typename Dps, typename Pref>
    dpoints_fwd_pixter(I& image,
		       const Dps& dps,
		       const Pref& p_ref);

    /// \brief Constructor (using a generalized pixel).
    /// 
    /// \param[in] pxl_ref Center (generalized) pixel to iterate around.
    /// \param[in] dps     An object (neighborhood or window) that can
    ///                    provide a set of delta-points.
    template <typename Dps, typename Pref>
    dpoints_fwd_pixter(const Generalized_Pixel<Pref>& pxl_ref,
		       const Dps& dps);

    /// Manipulation.
    /// \{
    /// Start an iteration.
    void start();
    /// Go to the next pixel.
    void next_();

    /// Invalidate the iterator.
    void invalidate();
    /// Test the iterator validity.
    bool is_valid() const;

    /// Force this iterator to update its location to take into
    /// account that its center point may have moved.
    void update();
    /// \}

    /// The value around which this iterator moves.
    const mln_value(I)& center_val() const;

  private:
    template <typename Dps>
    void init_(const Dps& dps);

  private:
    /// \brief Offset of each delta-point.
    ///
    /// offset_[0] is absolute, while other offsets are relative
    /// (i.e., offset_[i] is the memory difference to go from pixel
    /// i-1 to pixel i.
    std::vector<int> offset_;
    /// Current offset.
    typename std::vector<int>::const_iterator i_;

    /// \brief Reference value or pixel.
    ///
    /// One and only one of these pointers should be non-null.
    /// \{
    /// Reference value the image
    mln_qlf_value(I)** value_ref_;
    /// Reference pixel / point in the image
    const mln_psite(I)* p_ref_;
    /// \}
  };


  /*------------------------.
  | dpoints_bkd_pixter<I>.  |
  `------------------------*/

  /// \brief A generic backward iterator on the pixels of a
  /// dpoint-based window or neighborhood.
  /// 
  /// Parameter \c I is the image type.
  template <typename I>
  class dpoints_bkd_pixter
    : public Pixel_Iterator< dpoints_bkd_pixter<I> >,
      public internal::pixel_impl_< I, dpoints_bkd_pixter<I> >
  {
    typedef typename internal::pixel_impl_< I, dpoints_bkd_pixter<I> > super_;

  public:
    /// \brief Constructor (using an image).
    /// 
    /// \param[in] image The image to iterate over.
    /// \param[in] dps   An object (neighborhood or window) that can
    ///                  provide a set of delta-points.
    /// \param[in] p_ref Center (resp. reference) point of the
    ///                  neighborhood (resp. window).
    template <typename Dps, typename Pref>
    dpoints_bkd_pixter(I& image,
		       const Dps& dps,
		       const Pref& p_ref);

    /// \brief Constructor (using a generalized pixel).
    /// 
    /// \param[in] pxl_ref Center (generalized) pixel to iterate around.
    /// \param[in] dps     An object (neighborhood or window) that can
    ///                    provide a set of delta-points.
    template <typename Dps, typename Pref>
    dpoints_bkd_pixter(const Generalized_Pixel<Pref>& pxl_ref,
		       const Dps& dps);

    /// Manipulation.
    /// \{
    /// Start an iteration.
    void start();
    /// Go to the next pixel.
    void next_();

    /// Invalidate the iterator.
    void invalidate();
    /// Test the iterator validity.
    bool is_valid() const;

    /// Force this iterator to update its location to take into
    /// account that its center point may have moved.
    void update();
    /// \}

    /// The value around which this iterator moves.
    const mln_value(I)& center_val() const;

  private:
    template <typename Dps>
    void init_(const Dps& dps);

  private:
    /// \brief Offset of each delta-point.
    ///
    /// offset_[dps.ndpoints() - 1] is absolute, while other offsets
    /// are relative (i.e., offset_[i] is the memory difference to go
    /// from pixel i+1 to pixel i.
    std::vector<int> offset_;
    /// Current offset.
    typename std::vector<int>::const_reverse_iterator i_;

    /// \brief Reference value or pixel.
    ///
    /// One and only one of these pointers should be non-null.
    /// \{
    /// Reference value the image
    mln_qlf_value(I)** value_ref_;
    /// Reference pixel / point in the image
    const mln_psite(I)* p_ref_;
    /// \}
  };



#ifndef MLN_INCLUDE_ONLY

  /*------------------------.
  | dpoints_fwd_pixter<I>.  |
  `------------------------*/

  template <typename I>
  template <typename Dps, typename Pref>
  inline
  dpoints_fwd_pixter<I>::dpoints_fwd_pixter(I& image,
					    const Dps& dps,
					    const Pref& p_ref)
    : super_(image)
  {
    mln_precondition(image.has_data());
    p_ref_ = & exact(p_ref).to_site();
    value_ref_ = 0;
    init_(dps);
  }

  template <typename I>
  template <typename Dps, typename Pref>
  inline
  dpoints_fwd_pixter<I>::dpoints_fwd_pixter(const Generalized_Pixel<Pref>& pxl_ref_,
					    const Dps& dps)
    : super_(internal::force_exact<Pref>(pxl_ref_).ima())
  {
    const Pref& pxl_ref = internal::force_exact<Pref>(pxl_ref_);
    mln_precondition(pxl_ref.ima().has_data());
    p_ref_ = 0;
    // Potential promotion from (T**) to (const T**) shall be forced.
    value_ref_ = const_cast<mln_qlf_value(I)**>(pxl_ref.address_());
    init_(dps);
  }

  template <typename I>
  inline
  const mln_value(I)&
  dpoints_fwd_pixter<I>::center_val() const
  {
    mln_invariant(value_ref_ != 0 || p_ref_ != 0);
    if (p_ref_)
      return image_(*p_ref_);
    else
      return **value_ref_;
  }

  template <typename I>
  template <typename Dps>
  inline
  void
  dpoints_fwd_pixter<I>::init_(const Dps& dps)
  {
    for (unsigned i = 0; i < dps.ndpoints(); ++i)
      offset_.push_back(this->image_.delta_index(dps.dp(i)));
    // offset_[0] is absolute
    // other offsets are relative:
    if (dps.ndpoints() > 1)
      for (unsigned i = dps.ndpoints() - 1; i > 0; --i)
	offset_[i] -= offset_[i - 1];
    invalidate();
  }

  template <typename I>
  inline
  void
  dpoints_fwd_pixter<I>::update()
  {
    if (is_valid())
      {
	if (p_ref_)
	  this->value_ptr_ = & image_(*p_ref_) + *i_;
	else
	  this->value_ptr_ = * value_ref_ + *i_;
      }
  }

  template <typename I>
  inline
  void
  dpoints_fwd_pixter<I>::start()
  {
    i_ = offset_.begin();
    update();
  }

  template <typename I>
  inline
  void
  dpoints_fwd_pixter<I>::next_()
  {
    ++i_;
    if (is_valid())
      this->value_ptr_ += *i_;
  }

  template <typename I>
  inline
  bool
  dpoints_fwd_pixter<I>::is_valid() const
  {
    return i_ != offset_.end();
  }

  template <typename I>
  inline
  void
  dpoints_fwd_pixter<I>::invalidate()
  {
    i_ = offset_.end();
  }


  /*------------------------.
  | dpoints_bkd_pixter<I>.  |
  `------------------------*/

  template <typename I>
  template <typename Dps, typename Pref>
  inline
  dpoints_bkd_pixter<I>::dpoints_bkd_pixter(I& image,
					    const Dps& dps,
					    const Pref& p_ref)
    : super_(image)
  {
    mln_precondition(image.has_data());
    p_ref_ = & exact(p_ref).to_site();
    value_ref_ = 0;
    init_(dps);
  }

  template <typename I>
  template <typename Dps, typename Pref>
  inline
  dpoints_bkd_pixter<I>::dpoints_bkd_pixter(const Generalized_Pixel<Pref>& pxl_ref_,
					    const Dps& dps)
    : super_(internal::force_exact<Pref>(pxl_ref_).ima())
  {
    const Pref& pxl_ref = internal::force_exact<Pref>(pxl_ref_);
    mln_precondition(pxl_ref.ima().has_data());
    p_ref_ = 0;
    // Potential promotion from (T**) to (const T**) shall be forced.
    value_ref_ = const_cast<mln_qlf_value(I)**>(pxl_ref.address_());
    init_(dps);
  }

  template <typename I>
  inline
  const mln_value(I)&
  dpoints_bkd_pixter<I>::center_val() const
  {
    mln_invariant(value_ref_ != 0 || p_ref_ != 0);
    if (p_ref_)
      return image_(*p_ref_);
    else
      return **value_ref_;
  }

  template <typename I>
  template <typename Dps>
  inline
  void
  dpoints_bkd_pixter<I>::init_(const Dps& dps)
  {
    for (unsigned i = 0; i < dps.ndpoints(); ++i)
      offset_.push_back(this->image_.delta_index(dps.dp(i)));
    // offset_[ndpoints() - 1] is absolute
    // other offsets are relative:
    if (dps.ndpoints() > 1)
      for (unsigned i = 0; i < dps.ndpoints() - 1; ++i)
	offset_[i] -= offset_[i + 1];
    invalidate();
  }

  template <typename I>
  inline
  void
  dpoints_bkd_pixter<I>::update()
  {
    if (is_valid())
      {
	if (p_ref_)
	  this->value_ptr_ = & image_(*p_ref_) + *i_;
	else
	  this->value_ptr_ = * value_ref_ + *i_;
      }
  }

  template <typename I>
  inline
  void
  dpoints_bkd_pixter<I>::start()
  {
    i_ = offset_.rbegin();
    update();
  }

  template <typename I>
  inline
  void
  dpoints_bkd_pixter<I>::next_()
  {
    ++i_;
    if (is_valid())
      this->value_ptr_ += *i_;
  }

  template <typename I>
  inline
  bool
  dpoints_bkd_pixter<I>::is_valid() const
  {
    return i_ != offset_.rend();
  }

  template <typename I>
  inline
  void
  dpoints_bkd_pixter<I>::invalidate()
  {
    i_ = offset_.rend();
  }

#endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // MLN_CORE_DPOINTS_PIXTER_HH
