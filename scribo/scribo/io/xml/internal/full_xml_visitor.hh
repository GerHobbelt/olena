// Copyright (C) 2011 EPITA Research and Development Laboratory (LRDE)
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

#ifndef SCRIBO_IO_XML_INTERNAL_FULL_XML_VISITOR_HH
# define SCRIBO_IO_XML_INTERNAL_FULL_XML_VISITOR_HH

/// \file
///
/// XML serializer Visitor.

# include <fstream>

# include <QByteArray>

# include <scribo/core/internal/doc_xml_serializer.hh>
# include <scribo/core/document.hh>
# include <scribo/core/component_set.hh>
# include <scribo/core/paragraph_set.hh>
# include <scribo/core/object_groups.hh>
# include <scribo/core/object_links.hh>
# include <scribo/core/line_links.hh>
# include <scribo/core/line_info.hh>

# include <scribo/io/xml/internal/print_box_coords.hh>
# include <scribo/io/xml/internal/print_page_preambule.hh>

// Compression level 0-9. 9 is the best but is slow.
// 5 seems to be a good compromise.
# define COMPRESSION_LEVEL 5

namespace scribo
{

  namespace io
  {

    namespace xml
    {

      namespace internal
      {


	class full_xml_visitor : public doc_xml_serializer<full_xml_visitor>
	{
	public:
	  // Constructor
	  full_xml_visitor(std::ofstream& out);

	  // Visit overloads
	  template <typename L>
	  void visit(const document<L>& doc) const;

	  template <typename L>
	  void visit(const line_links<L>& llinks) const;

	  template <typename L>
	  void visit(const object_groups<L>& groups) const;

	  template <typename L>
	  void visit(const object_links<L>& links) const;

	  template <typename L>
	  void visit(const component_set<L>& comp_set) const;

	  void visit(const component_info& info) const;

	  template <typename L>
	  void visit(const paragraph_set<L>& parset) const;

	  template <typename L>
	  void visit(const line_info<L>& line) const;

	private: // Attributes
	  std::ofstream& output;
	};



# ifndef MLN_INCLUDE_ONLY


	inline
	full_xml_visitor::full_xml_visitor(std::ofstream& out)
	  : output(out)
	{
	}



	/// Document
	//
	template <typename L>
	void
	full_xml_visitor::visit(const document<L>& doc) const
	{
	  print_PAGE_preambule(output, doc, false);

	  // Text
	  if (doc.has_text())
	  {
	    const line_set<L>& lines = doc.lines();

	    // Save component/link/group information (Extension)
	    {
	      // Component set
	      lines.components().accept(*this);

	      // Object link
	      lines.links().accept(*this);

	      // Object group
	      lines.groups().accept(*this);
	    }
	    // End of EXTENSIONS

	    const paragraph_set<L>& parset = doc.paragraphs();

	    // Save paragraphs related information (Extension)
	    {
	      // General text information
	      output << "    <text_data nlines=\"" << lines.nelements() << "\" "
		   << " nparagraphs=\"" << parset.nelements() << "\" />" << std::endl;

	      // line_links
	      parset.links().accept(*this);
	    }

	    // Paragraph and lines
	    parset.accept(*this);
	  }


	  // Page elements (Pictures, ...)
	  if (doc.has_elements())
	  {
	    const component_set<L>& elts = doc.elements();
	    for_all_comps(e, elts)
	      if (elts(e).is_valid())
		elts(e).accept(*this);
	  }


	  // line seraparators
	  if (doc.has_line_seps())
	  {
	    const component_set<L>&
	      line_seps_comps = doc.line_seps_comps();

	    for_all_comps(c, line_seps_comps)
	      line_seps_comps(c).accept(*this);
	  }


	  // Whitespace seraparators
	  if (doc.has_whitespace_seps())
	  {
	    const component_set<L>&
	      whitespace_seps_comps = doc.whitespace_seps_comps();

	    for_all_comps(c, whitespace_seps_comps)
	      whitespace_seps_comps(c).accept(*this);
	  }

	  output << "  </page>" << std::endl;
	  output << "</pcGts>" << std::endl;

	}


	/// Line Links
	//
	template <typename L>
	void
	full_xml_visitor::visit(const line_links<L>& llinks) const
	{
	  output << "      <line_links>" << std::endl;
	  for_all_links(l, llinks)
	  {
	    output << "      <line_link"
		   << " from=\"" << l
		   << "\" to=\"" << llinks(l)
		   << "\"/>" << std::endl;
	  }
	  output << "      </line_links>" << std::endl;
	}


	/// Object Groups
        //
	template <typename L>
	void
	full_xml_visitor::visit(const object_groups<L>& groups) const
	{
	  output << "      <object_groups>" << std::endl;
	  for_all_groups(g, groups)
	  {
	    output << "      <group "
		 << " object_id=\"" << g
		 << "\" group_id=\"" << groups(g)
		 << "\"/>" << std::endl;
	  }
	  output << "      </object_groups>" << std::endl;
	}


	/// Object Links
	//
	template <typename L>
	void
	full_xml_visitor::visit(const object_links<L>& links) const
	{
	  output << "      <object_links>" << std::endl;
	  for_all_links(l, links)
	  {
	    output << "      <link"
		   << " from=\"" << l
		   << "\" to=\"" << links(l)
		   << "\"/>" << std::endl;
	  }
	  output << "      </object_links>" << std::endl;
	}


	/// Component Set
	//
	template <typename L>
	void
	full_xml_visitor::visit(const component_set<L>& comp_set) const
	{
	  output << "    <component_set nelements=\"" << comp_set.nelements()
		 << "\">" << std::endl;
	  for_all_comps(c, comp_set)
	  {
	    output << "      <component_info"
		   << " id=\"" << comp_set(c).id()
		   << "\" mass_center_x=\"" << comp_set(c).mass_center().col()
		   << "\" mass_center_y=\"" << comp_set(c).mass_center().row()
		   << "\" card=\"" <<  comp_set(c).card()
		   << "\" tag=\"" <<  comp_set(c).tag()
		   << "\" type=\"" <<  comp_set(c).type()
		   << "\" pmin_x=\"" << comp_set(c).bbox().pmin().col()
		   << "\" pmin_y=\"" << comp_set(c).bbox().pmin().row()
		   << "\" pmax_x=\"" << comp_set(c).bbox().pmax().col()
		   << "\" pmax_y=\"" << comp_set(c).bbox().pmax().row()
		   << "\"/>" << std::endl;
	  }


	  // Save labeled image
	  {
	    const L& lbl = comp_set.labeled_image();
	    output << "<labeled_image "
		   << " height=\"" << lbl.domain().height()
		   << "\" width=\"" << lbl.domain().width() << "\">"
		   << "<![CDATA[";

	    // FIXME: Try to avoid that!
	    border::resize(lbl, 0);
	    QByteArray lbl64((const char *)lbl.buffer(),
	    		     lbl.nelements() * sizeof(mln_value(L)));
	    lbl64 = qCompress(lbl64, COMPRESSION_LEVEL);
	    lbl64 = lbl64.toBase64();

	    output.write(lbl64.data(), lbl64.size());

	    output <<  "]]></labeled_image>" << std::endl;
	  }

	  // Save separators image
	  if (comp_set.has_separators())
	  {
	    const mln_ch_value(L,bool)& seps = comp_set.separators();
	    output << "<separators_image "
		   << " height=\"" << seps.domain().height()
		   << "\" width=\"" << seps.domain().width() << "\">"
		   << "<![CDATA[";

	    border::resize(seps, 0);
	    QByteArray seps64((const char *)seps.buffer(),
			      seps.nelements() * sizeof(bool));
	    seps64 = qCompress(seps64, COMPRESSION_LEVEL);
	    seps64 = seps64.toBase64();

	    output.write(seps64.data(), seps64.size());

	    output <<  "]]></separators_image>" << std::endl;
	  }

	  output << "</component_set>" << std::endl;
	}


	/// Component_info
	//
	inline
	void
	full_xml_visitor::visit(const component_info& info) const
	{
	  switch (info.type())
	  {
	    case component::WhitespaceSeparator:
	    {
	      output << "    <whitespace_separator_region id=\"wss"
		     << info.id()
		     << "\">" << std::endl;

	      internal::print_box_coords(output, info.bbox(), "      ");

	      output << "    </whitespace_separator_region>" << std::endl;
	      break;
	    }

	    case component::LineSeparator:
	    {
	      output << "    <separator_region id=\"sr" << info.id()
		     << "\" sep_orientation=\"0.000000\" "
		     << " sep_colour=\"Black\" "
		     << " sep_bgcolour=\"White\">" << std::endl;

	      internal::print_box_coords(output, info.bbox(), "      ");

	      output << "    </separator_region>" << std::endl;
	      break;
	    }


	    default:
	    case component::Image:
	    {
	      output << "    <image_region id=\"ir" << info.id()
		     << "\" img_colour_type=\"24_Bit_Colour\""
		     << " img_orientation=\"0.000000\" "
		     << " img_emb_text=\"No\" "
		     << " img_bgcolour=\"White\">" << std::endl;

	      internal::print_box_coords(output, info.bbox(), "      ");

	      output << "    </image_region>" << std::endl;
	      break;
	    }
	  }
	}

	/// Paragraph Set
	//
	template <typename L>
	void
	full_xml_visitor::visit(const paragraph_set<L>& parset) const
	{
	  const line_set<L>& lines = parset.lines();

	  for_all_paragraphs(p, parset)
	  {
	    const mln::util::array<line_id_t>& line_ids = parset(p).line_ids();

	    // FIXME: compute that information on the whole paragraph
	    // and use them here.
	    line_id_t fid = line_ids(0);
	    output << "    <text_region id=\"" << p
		   << "\" txt_orientation=\"" << lines(fid).orientation()
		   << "\" txt_reading_orientation=\"" << lines(fid).reading_orientation()
		   << "\" txt_reading_direction=\"" << lines(fid).reading_direction()
		   << "\" txt_text_type=\"" << lines(fid).type()
		   << "\" txt_reverse_video=\"" << (lines(fid).reverse_video() ? "true" : "false")
		   << "\" txt_indented=\"" << (lines(fid).indented() ? "true" : "false")
		   << "\" kerning=\"" << lines(fid).char_space();

	    // EXTENSIONS - Not officially supported
	    output << "\" baseline=\"" << lines(fid).baseline()
		   << "\" meanline=\"" << lines(fid).meanline()
		   << "\" x_height=\"" << lines(fid).x_height()
		   << "\" d_height=\"" << lines(fid).d_height()
		   << "\" a_height=\"" << lines(fid).a_height()
		   << "\" char_width=\"" << lines(fid).char_width();
	    // End of EXTENSIONS
	    output << "\">"
		   << std::endl;

	    internal::print_box_coords(output, parset(p).bbox(), "      ");


	    // EXTENSIONS - Not officially supported
	    for_all_paragraph_lines(lid, line_ids)
	    {
	      line_id_t l = line_ids(lid);

	      lines(l).accept(*this);
	    }

	    output << "    </text_region>" << std::endl;
	  }
	}


	template <typename L>
	void
	full_xml_visitor::visit(const line_info<L>& line) const
	{
	  if (line.has_text())
	  {
	    output << "        <line text=\"" << line.html_text() << "\" ";
	  }
	  else
	    output << "        <line " << std::endl;

	  output << "id=\"" << line.id()
		 << "\" txt_orientation=\"" << line.orientation()
		 << "\" txt_reading_orientation=\"" << line.reading_orientation()
		 << "\" txt_reading_direction=\"" << line.reading_direction()
		 << "\" txt_text_type=\"" << line.type()
		 << "\" txt_reverse_video=\"" << (line.reverse_video() ? "true" : "false")
		 << "\" txt_indented=\"" << (line.indented() ? "true" : "false")
		 << "\" kerning=\"" << line.char_space()
		 << "\" baseline=\"" << line.baseline()
		 << "\" meanline=\"" << line.meanline()
		 << "\" x_height=\"" << line.x_height()
		 << "\" d_height=\"" << line.d_height()
		 << "\" a_height=\"" << line.a_height()
		 << "\" char_width=\"" << line.char_width()
		 << "\">" << std::endl;

	  internal::print_box_coords(output, line.bbox(), "          ");

	  output << "          <compid_list>" << std::endl;

	  for_all_line_comps(c, line.components())
	    output << "            <compid value=\""
		   << line.components()(c) << "\" />" << std::endl;

	  output << "          </compid_list>" << std::endl;

	  output << "        </line>" << std::endl;
	}

#endif // MLN_INCLUDE_ONLY

      } // end of namespace scribo::io::xml::internal

    } // end of namespace scribo::io::xml

  } // end of namespace scribo::io

} // end of namespace scribo

# undef COMPRESSION_LEVEL

#endif // SCRIBO_IO_XML_INTERNAL_FULL_XML_VISITOR_HH
