#include <iostream>
#include <oln/core/2d/image2d.hh>
#include <oln/core/gen/rle_psite.hh>
#include <oln/core/gen/rle_image.hh>
#include <oln/debug/print.hh>
#include <oln/core/gen/rle_pset.hh>
#include <oln/core/gen/rle_encode.hh>

template <typename P>
void test(const oln::rle_pset<P>& my_set)
{
  typename oln::rle_pset<P>::piter run (my_set);
  for (run.start(); run.is_valid(); run.next())
    ;

   typename oln::rle_pset<P>::bkd_piter run2 (my_set);
   for (run2.start(); run2.is_valid(); run2.next())
     ;
}


int main()
{
  oln::point2d p(0,1), q(2,2), r(3, 0);
  oln::rle_pset<oln::point2d> my_set;
  oln::rle_image<oln::point2d, int> rle;
  oln::rle_image<oln::point2d, int> rle2;

  my_set.insert(p, 5);
  my_set.insert(q, 8);
  test(my_set);

  rle.insert(p, 5, 4);
  rle.insert(q, 8, 9);

  oln::image2d<int> ima2d (1, 5);
  ima2d(oln::point2d(0, 4)) = 5;

  rle2 = rle_encode(ima2d);

  return 0;
}
