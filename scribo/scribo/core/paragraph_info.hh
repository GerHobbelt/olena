// Copyright (C) 2011, 2013 EPITA Research and Development Laboratory
// (LRDE)
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

#ifndef SCRIBO_CORE_PARAGRAPH_INFO_HH
# define SCRIBO_CORE_PARAGRAPH_INFO_HH

# include <scribo/core/line_info.hh>
# include <scribo/core/line_links.hh>
# include <scribo/core/tag/paragraph.hh>
# include <mln/util/array.hh>
# include <mln/accu/shape/bbox.hh>

namespace scribo
{

  /*! \brief Paragraph structure information.

   */
  template <typename L>
  class paragraph_info
  {
  public:
    paragraph_info();
    paragraph_info(const line_links<L>& llinks);

    /*! \brief Add a new line to this paragraph

      This method is provided for an incremental construction.

      Once this method has been called, needs_stats_update() will
      return true until force_stats_update() is called.
     */
    void add_line(const line_info<L>& line);

    const mln::box2d& bbox() const;

    const line_info<L>& line(line_id_t id) const;

    const mln::util::array<line_id_t>& line_ids() const;

    unsigned nlines() const;

    const line_links<L>& llinks() const;

    // FIXME: add boldness?

    const mln::value::rgb8& color() const;
    void set_color_(const mln::value::rgb8& v);
    float color_reliability() const;
    void set_color_reliability_(float v);

    bool is_valid() const;
    void invalidate();

    bool needs_stats_update() const;
    void force_stats_update();

    void set_delta_baseline(const int delta_baseline);
    int delta_baseline() const;

    void fast_merge(paragraph_info<L>& info);

    void update_tag(paragraph::Tag tag);
    paragraph::Tag tag() const;

  private:
    mln::util::array<line_id_t> line_ids_;
    mln::accu::shape::bbox<mln_site(L)> bbox_;
    line_links<L> llinks_;

    mln::value::rgb8 color_;
    float color_reliability_;

    int delta_baseline_;
    paragraph::Tag tag_;
    bool is_valid_;
  };

  template <typename L>
  std::ostream& operator<<(std::ostream& ostr, const paragraph_info<L>& info);

  template <typename L>
  bool operator==(const paragraph_info<L>& lhs, const paragraph_info<L>& rhs);

# ifndef MLN_INCLUDE_ONLY


  template <typename L>
  paragraph_info<L>::paragraph_info()
    : tag_(paragraph::None), is_valid_(false)
  {
  }

  template <typename L>
  paragraph_info<L>::paragraph_info(const line_links<L>& llinks)
    : llinks_(llinks), tag_(paragraph::None), is_valid_(true)
  {
  }

  template <typename L>
  void
  paragraph_info<L>::add_line(const line_info<L>& line)
  {
    line_ids_.append(line.id());
    bbox_.take(line.bbox());

    // More data may need to be updated!
    tag_ = paragraph::Needs_Precise_Stats_Update;
    is_valid_ = true;
  }

  template <typename L>
  const mln::box2d&
  paragraph_info<L>::bbox() const
  {
    return bbox_.to_result();
  }

  template <typename L>
  const line_info<L>&
  paragraph_info<L>::line(line_id_t id) const
  {
    mln_precondition(is_valid());
    return llinks_.lines()(id);
  }

  template <typename L>
  const mln::util::array<line_id_t>&
  paragraph_info<L>::line_ids() const
  {
    return line_ids_;
  }

  template <typename L>
  unsigned
  paragraph_info<L>::nlines() const
  {
    return line_ids_.nelements();
  }

  template <typename L>
  const line_links<L>&
  paragraph_info<L>::llinks() const
  {
    return llinks_;
  }

  template <typename L>
  const mln::value::rgb8&
  paragraph_info<L>::color() const
  {
    return color_;
  }

  template <typename L>
  void
  paragraph_info<L>::set_color_(const mln::value::rgb8& v)
  {
    color_ = v;
  }

  template <typename L>
  float
  paragraph_info<L>::color_reliability() const
  {
    return color_reliability_;
  }

  template <typename L>
  void
  paragraph_info<L>::set_color_reliability_(float v)
  {
    color_reliability_ = v;
  }

  template <typename L>
  bool
  paragraph_info<L>::is_valid() const
  {
    return llinks_.is_valid() && is_valid_;
  }

  template <typename L>
  void
  paragraph_info<L>::invalidate()
  {
    is_valid_ = false;
  }

  template <typename L>
  bool
  paragraph_info<L>::needs_stats_update() const
  {
    return tag_ == paragraph::Needs_Precise_Stats_Update;
  }

  template <typename L>
  void
  paragraph_info<L>::force_stats_update()
  {
    if (!needs_stats_update())
      return;

    const line_set<L>& lines = llinks_.lines();

    // Update color

    mln::accu::stat::mean<mln::value::int_u<8> >
      color_red,
      color_green,
      color_blue;

    float
      sum2_red = 0,
      sum2_green = 0,
      sum2_blue = 0;

    for_all_elements(e, line_ids_)
    {
      unsigned lid = line_ids_(e);

      color_red.take(lines(lid).color().red());
      color_green.take(lines(lid).color().green());
      color_blue.take(lines(lid).color().blue());
      sum2_red += mln::math::sqr<unsigned>(lines(lid).color().red());
      sum2_green += mln::math::sqr<unsigned>(lines(lid).color().green());
      sum2_blue += mln::math::sqr<unsigned>(lines(lid).color().blue());
    }

    color_ = mln::value::rgb8(color_red.to_result(),
			      color_green.to_result(),
			      color_blue.to_result());

    float
      var_red = sum2_red / (float)line_ids_.nelements()
      - mln::math::sqr<float>(color_red.to_result()),
      var_green = sum2_green / (float)line_ids_.nelements()
      - mln::math::sqr<float>(color_green.to_result()),
      var_blue = sum2_blue / (float)line_ids_.nelements()
      - mln::math::sqr<float>(color_blue.to_result());

    color_reliability_ = std::sqrt(std::max(var_red,
					    std::max(var_green, var_blue)));

    // FIXME: Update paragraph stats


    // Compute paragraph's delta baseline
    stats< float > delta(nlines());
    const unsigned nelements = line_ids_.nelements();

    for (unsigned i = 0; i < nelements; ++i)
    {
      const line_id_t& current_id = line_ids_(i);

      if (llinks_(current_id) != current_id)
      {
	const line_info<L>& current_line = lines(current_id);
	const line_info<L>& left_line = lines(llinks_(current_id));

	delta.take(left_line.baseline() - current_line.baseline());
      }
    }

    int median = delta.median();

    if (!median)
      median = lines(line_ids_(0)).x_height();

    set_delta_baseline(median);


    // Update tag
    tag_ = paragraph::None;
  }

  template <typename L>
  void
  paragraph_info<L>::set_delta_baseline(const int delta_baseline)
  {
    delta_baseline_ = delta_baseline;
  }

  template <typename L>
  int
  paragraph_info<L>::delta_baseline() const
  {
    return delta_baseline_;
  }

  template <typename L>
  void
  paragraph_info<L>::fast_merge(paragraph_info<L>& other)
  {
    tag_ = paragraph::Needs_Precise_Stats_Update;
    other.update_tag(paragraph::Merged);
    other.invalidate();

    // Merge bboxes.
    bbox_.take(other.bbox());

    // Update delta_baseline
    // FIXME: delta base line should be updated correctly!!
    set_delta_baseline(std::max(other.delta_baseline_, delta_baseline_));

    line_ids_.append(other.line_ids());
  }

  template <typename L>
  void
  paragraph_info<L>::update_tag(paragraph::Tag tag)
  {
    tag_ = tag;
  }

  template <typename L>
  paragraph::Tag
  paragraph_info<L>::tag() const
  {
    return tag_;
  }

  template <typename L>
  bool
  operator==(const paragraph_info<L>& lhs, const paragraph_info<L>& rhs)
  {



    return
      lhs.line_ids() == rhs.line_ids()
      && lhs.bbox() == rhs.bbox()
      && lhs.llinks() == rhs.llinks()
      && lhs.color() == rhs.color()
      && lhs.color_reliability() == rhs.color_reliability()
      && lhs.needs_stats_update() == rhs.needs_stats_update();
  }

  template <typename L>
  std::ostream&
  operator<<(std::ostream& ostr, const paragraph_info<L>& info)
  {
    return ostr << "paragraph_info("
		<< "line_ids=" << info.line_ids()
		<< ", bbox=" << info.bbox()
		<< ", color=" << info.color()
		<< ", color_reliability=" << info.color_reliability()
		<< ")" << std::endl;
  }


# endif // ! MLN_INCLUDE_ONLY

} // end of namespace scribo

#endif // ! SCRIBO_CORE_PARAGRAPH_INFO_HH
