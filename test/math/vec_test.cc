#include "kuro/math/vec.hh"

#include <gtest/gtest.h>

using namespace kuro;

TEST(vec_test, ctor) 
{
  Vec3i v3i(1, 2, 3);
  Vec3f v3f(1., 2., 3.);
}

TEST(vec_test, cross_product)
{
  Vec3i a(1, 2, 3);
  Vec3i b(4, 2, 3);

  auto c = CrossProduct3(a, b);
  EXPECT_TRUE(IsOrthogonal(c, a));
  EXPECT_TRUE(IsOrthogonal(c, b));
}

TEST(vec_test, ortho)
{
  Vec<uint8_t, 3> a(1, 2, 3);
  Vec<uint8_t, 3> b(1, 2, 3);

  EXPECT_TRUE(!IsOrthogonal(a, b));
}
