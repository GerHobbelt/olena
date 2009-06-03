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

#ifndef MLN_DISPLAY_REMOVE_HH
# define MLN_DISPLAY_REMOVE_HH

/// \file mln/display/remove.hh
///
/// Remove all temporary files create by display::save.

# include <map>
# include <string>
# include <mln/trace/all.hh>


namespace mln
{

  namespace display
  {

    /// Remove all temporary images which are created by the routine
    /// save.
    void remove();


# ifndef MLN_INCLUDE_ONLY

    namespace impl
    {

      inline
      void
      remove()
      {
	trace::entering("display::impl::remove");

	for (std::map<void*, std::string>::const_iterator it = map_saved_image_tmp_.begin ();
	     it != map_saved_image_tmp_.end ();
	     ++it)
	  {
	    std::string s = "rm -f " + (*it).second;
	    system (s.c_str ());
	  }

	trace::exiting("display::impl::remove");
      }

    } // end of namespace mln::display::impl

    /// Facade.
    inline
    void
    remove()
    {
      trace::entering("display::remove");

      impl::remove();

      trace::exiting("display::remove");
    }

# endif // !MLN_INCLUDE_ONLY

  } // end of namespace mln::display

} // end of namespace mln


#endif //  ! MLN_DISPLAY_REMOVE_HH
