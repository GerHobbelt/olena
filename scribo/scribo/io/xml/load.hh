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

#ifndef SCRIBO_IO_XML_LOAD_HH
# define SCRIBO_IO_XML_LOAD_HH

/// \file
///
/// \brief Load document information from XML.

# include <QtXml>

# include <libgen.h>
# include <fstream>
# include <sstream>

# include <map>

# include <mln/core/image/image2d.hh>

# include <mln/data/wrap.hh>
# include <mln/value/int_u8.hh>
# include <mln/io/pgm/save.hh>
# include <mln/io/pbm/save.hh>

# include <scribo/core/document.hh>
# include <scribo/core/component_set.hh>
# include <scribo/core/line_set.hh>
# include <scribo/core/line_info.hh>

# include <scribo/convert/from_base64.hh>

namespace scribo
{

  namespace io
  {

    namespace xml
    {
      using namespace mln;

      /*! \brief Load document information from XML.

	We use a XML Schema part of the PAGE (Page Analysis and Ground
	truth Elements) image representation framework.

	This schema was used in the Page Segmentation COMPetition
	(PSCOMP) for ICDAR 2009.

	Its XSD file is located here:
	http://schema.primaresearch.org/PAGE/gts/pagecontent/2009-03-16/pagecontent.xsd

      */
      template <typename L>
      void
      load(document<L>& doc, const std::string& input_name);


# ifndef MLN_INCLUDE_ONLY

      namespace internal
      {


	enum Mode
	{
	  None,
	  ComponentSet,
	  ComponentInfo,
	  LabeledImage,
	  SeparatorsImage,
	  ObjectLinks,
	  ObjectGroups,
	  Point,
	  Link,
	  Group,
	  Line,
	  LineLinks,
	  LineLink,
	  TextData,
	  TextRegion,
	  CompIdList,
	  CompId,
	  Page
	};


	struct ModeData
	{
	  const char *name;
	  Mode mode;
	};


	static const ModeData mode_data[] = {
	  { "component_set", ComponentSet },
	  { "component_info", ComponentInfo },
	  { "labeled_image", LabeledImage },
	  { "separators_image", SeparatorsImage },
	  { "object_links",  ObjectLinks },
	  { "object_groups", ObjectGroups },
	  { "point", Point },
	  { "link", Link },
	  { "group", Group },
	  { "line",  Line },
	  { "line_links",  LineLinks },
	  { "line_link",  LineLink },
	  { "text_data", TextData },
	  { "text_region", TextRegion },
	  { "compid_list", CompIdList },
	  { "compid", CompId },
	  { "page", Page },
	  { 0, None }
	};


	template <typename L>
	class xml_handler : public QXmlDefaultHandler
	{

	  typedef mln_ch_value(L,bool) B;

	public:
	  xml_handler() : current_paragraph_id(1) { lines_data.append(line_info<L>()); } // line info id starts from 1.

	  virtual
	  bool
	  startElement(const QString& /*namespaceURI*/, const QString& /*localName*/,
		       const QString& qName, const QXmlAttributes& atts )
	  {
	    mode.push(find_mode(qName));

	    switch (mode.top())
	    {

	      // Component Set
	      case ComponentSet:
	      {
		comp_set_data = new scribo::internal::component_set_data<L>();
		// qDebug() << qName << " - atts.value(\"nelements\").toInt() = " << atts.value("nelements").toInt();;
		comp_set_data->soft_init(atts.value("nelements").toInt());
	      }
	      break;


	      // Component Info
	      case ComponentInfo:
	      {
		component_info info(atts.value("id").toInt(),
				    mln::make::box2d(atts.value("pmin_y").toInt(),
						     atts.value("pmin_x").toInt(),
						     atts.value("pmax_y").toInt(),
						     atts.value("pmax_x").toInt()),
				    mln::point2d(atts.value("mass_center_y").toInt(),
						 atts.value("mass_center_x").toInt()),
				    atts.value("card").toInt());


		info.update_tag(component::str2tag(atts.value("tag").toUtf8().constData()));
		info.update_type(component::str2type(atts.value("type").toUtf8().constData()));

		comp_set_data->infos_.append(info);
	      }
	      break;


	      // Object links
	      case ObjectLinks:
	      {
		// qDebug() << "object_links created";
		links = object_links<L>(components);
	      }
	      break;


	      // Object groups
	      case ObjectGroups:
	      {
		// qDebug() << "object_groups created";
		groups = object_groups<L>(links);
	      }
	      break;


	      // Text data
	      case TextData:
	      {
		// qDebug() << "TextData";

		// Reserve space for line data.
		lines_data.resize(atts.value("nlines").toInt() + 1);
		// qDebug() << "line_set created";
		lines = line_set<L>(groups, lines_data);
		llinks = line_links<L>(lines);
		par_data = new scribo::internal::paragraph_set_data<L>(llinks, atts.value("nparagraphs").toInt());
		// std::cout << par_data->pars_.nelements() << " - " << llinks.nelements() << " - " << lines.nelements() << std::endl;
	      }
	      break;


	      // Text Region
	      case TextRegion:
	      {
		// qDebug() << "TextRegion";

		current_paragraph = paragraph_info<L>(llinks);
	      }
	      break;


	      // Line link
	      case LineLink:
	      {
		llinks(atts.value("from").toInt()) = atts.value("to").toInt();
	      }
	      break;


	      // Line
	      case Line:
	      {
		current_line_id = atts.value("id").toInt();

		line_data = new scribo::internal::line_info_data<L>(lines, mln::util::array<component_id_t>());

		line_data->holder_ = lines;
		line_data->text_ = atts.value("text").toUtf8().constData();

		line_data->hidden_ = false;
		line_data->tag_ = static_cast<line::Tag>(atts.value("tag").toInt());

		line_data->baseline_ = atts.value("baseline").toInt();
		line_data->meanline_ = atts.value("meanline").toInt();
		line_data->x_height_ = atts.value("x_height").toInt();
		line_data->d_height_ = atts.value("d_height").toInt();
		line_data->a_height_ = atts.value("a_height").toInt();
		line_data->char_space_ = atts.value("kerning").toInt();
		line_data->char_width_ = atts.value("char_width").toInt();
		line_data->word_space_ = 0;

		line_data->reading_direction_ = line::LeftToRight;
		line_data->type_ = line::str2type(atts.value("txt_text_type").toAscii().constData());
		line_data->reverse_video_ = (atts.value("txt_reverse_video") == "false" ? false : true);
		line_data->orientation_ = 0;
		line_data->reading_orientation_ = atts.value("txt_reading_orientation").toInt();
		line_data->indented_ = (atts.value("txt_indented") == "false" ? false : true);

		bbox.init();
	      }
	      break;


	      // CompIdList
	      case CompIdList:
	      {

	      }
	      break;


	      // CompId
	      case CompId:
	      {
		line_data->components_.append(atts.value("value").toInt());
	      }
	      break;


	      // Point
	      case Point:
	      {
		point2d p(atts.value("y").toInt(), atts.value("x").toInt());
		bbox.take(p);
	      }
	      break;


	      // Labeled Image
	      case LabeledImage:
	      {
		width = atts.value("width").toInt();
		height = atts.value("height").toInt();
		comp_set_data->ima_ = L(mln::make::box2d(height, width), 0); // No border
	      }
	      break;


	      // Separator Image
	      case SeparatorsImage:
	      {
		width = atts.value("width").toInt();
		height = atts.value("height").toInt();
		comp_set_data->separators_ = B(mln::make::box2d(height, width), 0); // No border
	      }
	      break;


	      // Link
	      case Link:
	      {
		links(atts.value("from").toInt()) = atts.value("to").toInt();
	      }
	      break;


	      // Group
	      case Group:
	      {
		groups(atts.value("object_id").toInt()) = atts.value("group_id").toInt();
	      }
	      break;


	      // DEFAULT
	      default:
		;
	    }

	    return true;
	  }


	  virtual
	  bool
	  endElement(const QString& /*namespaceURI*/, const QString& /*localName*/, const QString& /*qName*/)
	  {
	    switch(mode.top())
	    {
	      // Component set
	      case ComponentSet:
	      {
		// qDebug() << "Component set done";
		components = component_set<L>(comp_set_data);
	      }
	      break;

	      // Line
	      case Line:
	      {
		// qDebug() << "Line done";
		line_data->bbox_ = bbox;
		lines_data(current_line_id) = line_info<L>(current_line_id, line_data);
		lines_data(current_line_id).update_ebbox();

		// Add this line to the current paragraph.
		current_paragraph.add_line(lines_data(current_line_id));
	      }
	      break;

	      // TextRegion
	      case TextRegion:
	      {
		// qDebug() << TextRegion;
		par_data->pars_(current_paragraph_id++) = current_paragraph;
	      }
	      break;

	      // Page
	      case Page:
	      {
		// qDebug() << "Page done";
		lines.update_line_data_(lines_data);
		parset = paragraph_set<L>(par_data);
	      }
	      break;

	      // DEFAULT
	      default:
		;

	    }

	    mode.pop();
	    return true;
	  }



	  bool characters(const QString & ch)
	  {
	    switch (mode.top())
	    {
	      case LabeledImage:
	      {
		scribo::convert::from_base64(ch, comp_set_data->ima_);
	      }
	      break;

	      case SeparatorsImage:
	      {
		scribo::convert::from_base64(ch, comp_set_data->separators_);
	      }
	      break;

	      default:
		;
	    }

	    return true;
	  }


//	private: // Methods

	  Mode find_mode(const QString& qName)
	  {
	    for (int i = 0; mode_data[i].name; ++i)
	      if (mode_data[i].name == qName)
		return mode_data[i].mode;
	    return None;
	  }


//	private: // Attributes

	  QStack<Mode> mode;

	  // Shape
	  accu::shape::bbox<point2d> bbox;

	  unsigned width;
	  unsigned height;

	  // Components
	  mln::util::tracked_ptr<scribo::internal::component_set_data<L> > comp_set_data;
	  component_set<L> components;

	  object_links<L> links;
	  object_groups<L> groups;

	  // Lines
	  unsigned current_line_id;
	  scribo::internal::line_info_data<L> *line_data;

	  line_links<L> llinks;

	  unsigned current_paragraph_id;
	  paragraph_info<L> current_paragraph;
	  scribo::internal::paragraph_set_data<L> *par_data;
	  paragraph_set<L> parset;

	  mln::util::array<line_info<L> > lines_data;
	  line_set<L> lines;
	};










	template <typename L>
	void
	load_extended(document<L>& doc,
		      const std::string& output_name)
	{
	  xml_handler<L> handler;
	  QXmlSimpleReader reader;
	  reader.setContentHandler(&handler);

	  QFile file(output_name.c_str());
	  if (!file.open(QFile::ReadOnly | QFile::Text))
	  {
	    qDebug() << "Cannot read file";
	    return;
	  }

	  QXmlInputSource xmlInputSource(&file);
	  if (reader.parse(xmlInputSource))
	    qDebug() << "Loaded successfuly";

	  doc.set_paragraphs(handler.parset);
	}

      } // end of namespace scribo::io::xml::internal


      // FACADE

      template <typename L>
      void
      load(document<L>& doc,
	   const std::string& output_name)
      {
	internal::load_extended(doc, output_name);
      }


# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace scribo::io::xml

  } // end of namespace scribo::io

} // end of namespace scribo


#endif // ! SCRIBO_IO_XML_LOAD_HH

