// Copyright (C) 2010, 2011, 2013 EPITA Research and Development
// Laboratory (LRDE)
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


#include <libgen.h>
#include <iostream>

#include <mln/core/image/image2d.hh>

#include <mln/io/magick/load.hh>
#include <mln/io/pbm/save.hh>

#include <mln/value/rgb8.hh>

#include <scribo/debug/option_parser.hh>

#include <scribo/toolchain/text_in_doc_preprocess.hh>


static const scribo::debug::arg_data arg_desc[] =
{
  { "input.*", "An image." },
  { "output.pbm", "Binary preprocess image." },
  {0, 0}
};


// --enable/disable-<name>
static const scribo::debug::toggle_data toggle_desc[] =
{
  // name, description, default value
  { "fg-extraction", "Detect and slit foreground/background components. (default: disabled)", false },
  {0, 0, false}
};


// --<name> <args>
static const scribo::debug::opt_data opt_desc[] =
{
  // name, description, arguments, check args function, number of args, default arg
  { "lambda", "Set the maximum area of the background objects. It is only useful if fg-extraction is enabled.", "<size>",
    0, 1, "0" },
  { "verbose", "Enable verbose mode", 0, 0, 0, 0 },
  {0, 0, 0, 0, 0, 0}
};



int main(int argc, char* argv[])
{
  using namespace scribo;
  using namespace mln;

  scribo::debug::option_parser options(arg_desc, toggle_desc, opt_desc);

  if (!options.parse(argc, argv))
    return 1;

  image2d<value::rgb8> input_rgb;
  io::magick::load(input_rgb, options.arg("input.*"));

  unsigned lambda = atoi(options.opt_value("lambda").c_str());
  bool fg_extraction = options.is_enabled("fg-extraction");
  bool verbose = options.is_set("verbose");

  image2d<bool> output;

  output = toolchain::text_in_doc_preprocess(input_rgb, fg_extraction,
					     lambda, 0.34, false, verbose);

  mln::io::pbm::save(output, options.arg("output.pbm"));
}
