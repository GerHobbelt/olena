#ifndef OLENA_CONVERT_RGBXYZ_HH
# define OLENA_CONVERT_RGBXYZ_HH

# include <mln/core/image_if_value.hh>
# include <mln/core/inplace.hh>
# include <mln/core/w_window2d_int.hh>
# include <mln/display/show.hh>
# include <mln/io/ppm/save.hh>
# include <mln/display/save_and_show.hh>
# include <mln/level/fill.hh>

# include "my_xyz.hh"

namespace mln {
  namespace convert {
    struct f_rgb_to_xyz
    {
      struct value::xyz<8>
      doit(const struct value::rgb<8> rgb) const
      {
	struct value::xyz<8> xyz;

	xyz.x(0.490 * rgb.red() + 0.310 * rgb.green() + 0.200 * rgb.blue());
	xyz.y(0.177 * rgb.red() + 0.812 * rgb.green() + 0.011 * rgb.blue());
	xyz.z(0.010 * rgb.green() + 0.990 * rgb.blue());

	return (xyz);
_      }
    };
    
    struct f_xyz_to_rgb
    {
      struct value::rgb<8>
      doit(const struct value::xyz<8> xyz) const
      {
	struct value::rgb<8> rgb;
	int r;
	int g;
	int b;

	r = int(2.365 * xyz.x() - 0.896 * xyz.y() - 0.468 * xyz.z());
	g = int(-0.515 * xyz.x() + 1.425 * xyz.y() + 0.089 * xyz.z());
	b = int(0.005 * xyz.x() - 0.014 * xyz.y() + 1.01 * xyz.z());

	struct value::rgb<8> rgb(r, g, b); 
	
	return (rgb);
      }
    };
  }
}

#endif // OLENA_CONVERT_RGBXYZ_HH
