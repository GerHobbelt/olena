// Copyright (C) 2010 EPITA Research and Development Laboratory (LRDE)
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

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include <iostream>

#include <mln/core/image/image2d.hh>
#include <mln/io/pbm/load.hh>

#include <scribo/text/recognition.hh>

#include <scribo/debug/usage.hh>


const char *args_desc[][2] =
{
  { "input.pbm", "A binary image. 'True' for objects, 'False'\
for the background." },
  { "lang", "Must be set to \"eng\", \"fra\", \"deu\", \"spa\", \"ita\" (Default \"fra\")" },
  {0, 0}
};



int main(int argc, char* argv[])
{
  using namespace scribo;
  using namespace mln;

  if (argc != 2 && argc != 3)
    return scribo::debug::usage(argv,
				"Text recognition",
				"input.pbm [lang]",
				args_desc);

  trace::entering("main");

  image2d<bool> input;
  io::pbm::load(input, argv[1]);


  std::string str = argv[2];
  const char *lang;
  if (argc < 3 || (str != "eng" && str != "fra"
		   && str != "deu" && str != "spa"
		   && str != "ita"))
    lang = "fra";
  else
    lang = argv[2];

  scribo::text::recognition(input, lang);

  trace::exiting("main");
}
