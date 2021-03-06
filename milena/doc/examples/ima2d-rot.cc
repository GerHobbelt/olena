#include <mln/core/image/image2d.hh>
#include <mln/core/image/dmorph/extended.hh>
#include <mln/core/var.hh>
#include <mln/core/routine/extend.hh>

#include <mln/value/rgb8.hh>

#include <mln/border/fill.hh>

#include <mln/literal/colors.hh>

#include <mln/data/fill.hh>

#include <mln/draw/box.hh>

#include <mln/fun/x2x/rotation.hh>

#include <mln/algebra/vec.hh>

#include <doc/tools/sample_utils.hh>

int main()
{
  using namespace mln;

  // \{
  border::thickness = 30;

  // Declare the image to be rotated.
  image2d<value::rgb8> ima1_(220, 220);
  data::fill(ima1_, literal::cyan);
  border::fill(ima1_, literal::yellow);
  // Set an infinite extension.
  mln_VAR(ima1, extend(ima1_, pw::cst(literal::yellow)));

  // Declare the output image.
  image2d<value::rgb8> ima2(220, 220);
  data::fill(ima2, literal::cyan);
  border::fill(ima2, literal::yellow);

  box2d extended_domain= ima1.domain();
  extended_domain.enlarge(border::thickness);

  // Draw the domain bounding box
  draw::box(ima1, geom::bbox(ima1_), literal::red);
  // Save the image, including its border.
  doc::ppmsave(ima1 | extended_domain, "ima2d-rot");

  // Define and apply a point-wise rotation
  fun::x2x::rotation<2,float> rot1(0.5, literal::zero);
  image2d<value::rgb8>::fwd_piter p(ima1.domain());
  for_all(p)
  {
    algebra::vec<2,float> pv = p.to_site().to_vec();
    algebra::vec<2,float> v = rot1.inv()(pv);
    ima2(p) = ima1(v);
  }

  draw::box(ima2, ima2.bbox(), literal::red);
  doc::ppmsave(extended_to(ima2, extended_domain), "ima2d-rot");
  // \}

}
