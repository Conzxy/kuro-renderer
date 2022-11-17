#include "kuro/img/tga_image.hh"

#include <gtest/gtest.h>

using namespace kuro;

TEST (tga_image_test, read_from)
{
  TgaImage image;
  image.EnableDebug(true);
  ASSERT_TRUE(image.ReadFrom("../../bin/obj/diablo3_pose/diablo3_pose_diffuse.tga"));
  EXPECT_TRUE(image.WriteTo("tga_image_test_read_from.tga"));
  EXPECT_TRUE(image.WriteTo("rle_tga_image_test_read_from.tga", true));

  TgaImage image2;
  ASSERT_TRUE(image2.ReadFrom("rle_tga_image_test_read_from.tga"));
  EXPECT_TRUE(image2.WriteTo("tga_image_test_read_from2.tga"));
  image2.WriteTo("tga_image_test_read_from_br.tga", false, TgaImage::BOTTOM_RIGHT);
}
