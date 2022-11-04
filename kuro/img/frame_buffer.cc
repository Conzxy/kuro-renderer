#include "frame_buffer.hh"

#include <stdio.h>
#include <assert.h>
#include <string.h>

using namespace kuro;

void FrameColor::Print() const noexcept
{
  printf("(0x%x, 0x%x, 0x%x, 0x%x)\n", r, g, b, a);
}

const FrameColor FrameColor::red(0xff, 0, 0);
const FrameColor FrameColor::green(0, 0xff, 0);
const FrameColor FrameColor::blue(0, 0, 0xff);
const FrameColor FrameColor::white(0xff, 0xff, 0xff);
const FrameColor FrameColor::black(0, 0, 0);

int FrameBuffer::ImageType2BytesPerPixel(ImageType t) noexcept
{
  switch (t) {
    case FrameBuffer::IMAGE_TYPE_RGB:
      return 4;
  }

  return (int)t;
}

void FrameBuffer::SetPixel(int x, int y, FrameColor const &c) noexcept
{
  assert(x >= 0 && x <= width_ &&
         y >= 0 && y <= height_);
  const auto bpp = GetBytesPerPixel();
  memcpy(&data_[(x+y*width_)*bpp], &c, bpp);
}

FrameColor FrameBuffer::GetPixel(int x, int y) noexcept
{
  assert(x >= 0 && x <= width_ &&
         y >= 0 && y <= height_);
  const auto bpp = GetBytesPerPixel();
  FrameColor color;
  memcpy(&color, &data_[(x+y*width_)*bpp], bpp);
  return color;
}

void FrameBuffer::ClearAllPixel() noexcept
{
  memset(&data_[0], 0, data_.size());
}
