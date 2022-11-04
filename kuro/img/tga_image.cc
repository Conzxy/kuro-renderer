#include "tga_image.hh"

#include <string.h>
#include <assert.h>

#include "kuro/util/file.hh"

using namespace kuro;

const TgaColor TgaColor::white{ 0xff, 0xff, 0xff };
const TgaColor TgaColor::red{ 0xff, 0, 0 };

void TgaColor::Print() const noexcept
{
  printf("(0x%x, 0x%x, 0x%x, 0x%x)\n", r, g, b, a);
}

TgaImage::TgaImage(int w, int h, ImageType t)
  : width_(w)
  , height_(h)
  , image_type_(t)
  , image_data_(bytes_per_pixel() * w * h, 0)
{
}

TgaImage::~TgaImage() noexcept
{
}

bool TgaImage::WriteTo(char const *path, ImageOriginOrder order, bool rle) noexcept
{
  File out;
  if (!out.Open(path, File::TRUNC)) {
    fprintf(stderr, "Failed to write tga file: %s\n", path);
    return false;
  }

  // FIXME little endian
  TgaHeader header;
  header.pixel_depth = bits_per_pixel();
  header.height = height_;
  header.width = width_;
  switch (image_type_) {
    case NO_IMAGE_DATA:
      header.image_type = 0;
      break;
    case GRAYSCALE:
      header.image_type = rle ? 11 : 3;
      break;
    case RGB:
    case RGBA:
      header.image_type = rle ? 8 : 2;
      break;
  }
  header.image_descriptor |= (uint8_t)order;

  out.Write(&header, sizeof(header));

  /*
   * After header:
   * Image/Color Map data:
   * |++++++++++++++++|
   * | image ID       | variable
   * | color map data | variable
   * | image data     | variable
   * |++++++++++++++++|
   */
  
  /* Don't care color map data and image ID now */
  out.Write(image_data_.data(), image_data_.size());
  
  /*
   * Developer area
   * and Extension area ignored also
   */

  
  /**
   * Append footer to indicates this is new version tga format
   */
  TgaFooter footer;
  memcpy(footer.signature, "TRUEVISION-XFILE", sizeof footer.signature);

  out.Write(&footer, sizeof footer);
  return true;
}

bool TgaImage::ReadFrom(char const *path) noexcept
{
  File in;
  if (!in.Open(path, File::READ)) {
    fprintf(stderr, "Failed to read tga file: %s\n", path);
    return false;
  }
  
  // TODO Implement this! 
  return true;
}

void TgaImage::SetPixel(int x, int y, TgaColor const &color) noexcept
{
  assert(x > 0 && x < width_ &&
         y > 0 && y < height_);

  const auto bpp = bytes_per_pixel();
  memcpy(&image_data_[(x+y*width_)*bpp], &color, bpp);
}

TgaColor TgaImage::GetPixel(int x, int y) noexcept
{
  // FIXME Use ASSERT(work in release mode also)
  assert(x > 0 && x < width_ &&
         y > 0 && y < height_);

  TgaColor ret;
  const auto bpp = bytes_per_pixel();
  memcpy(&ret, &image_data_[(x+y*width_)*bpp], bpp);

  return ret;
}

