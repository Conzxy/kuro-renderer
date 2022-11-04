#ifndef KURO_IMG_FRAME_BUFFER_H__
#define KURO_IMG_FRAME_BUFFER_H__

#include <vector>
#include <stdint.h>

namespace kuro {

struct FrameColor {
  static const FrameColor red;
  static const FrameColor green;
  static const FrameColor blue;
  static const FrameColor white;
  static const FrameColor black;

  FrameColor() = default;

  FrameColor(uint8_t _r, uint8_t _g, uint8_t _b, uint8_t _a = 255)
    : b(_b)
    , g(_g)
    , r(_r)
    , a(_a)
  {
  }

  void Print() const noexcept;

  uint8_t b = 0;
  uint8_t g = 0;
  uint8_t r = 0;
  uint8_t a = 255; // Opacity 100% default
};

class FrameBuffer {
 public:
  enum ImageType {
    IMAGE_TYPE_INVALID = 0,
    IMAGE_TYPE_RGB = 3,
    IMAGE_TYPE_RGBA = 4,
  };
  

  FrameBuffer(int w, int h, ImageType t)
    : width_(w)
    , height_(h)
    , bpp_(ImageType2BytesPerPixel(t))
    , type_(t)
    , data_((w+1) * (h+1) * GetBytesPerPixel(), 0) 
  {
  }

  ~FrameBuffer() noexcept
  {
  }

  void SetPixel(int x, int y, FrameColor const &c) noexcept;
  FrameColor GetPixel(int x, int y) noexcept;
  
  void ClearAllPixel() noexcept;

  int GetBytesPerPixel() const noexcept
  {
    return bpp_;
  }

  int GetBitsPerPixel() const noexcept
  {
    return GetBytesPerPixel() << 3;
  }
  
  int GetWidth() const noexcept { return width_; }
  int GetHeight() const noexcept { return height_; }
  uint8_t const *GetRawData() const noexcept { return data_.data(); }

  ImageType GetImageType() const noexcept { return type_; }
 private:
  static int ImageType2BytesPerPixel(ImageType t) noexcept;

  int width_;
  int height_;
  int bpp_; // bytes per pixel
  ImageType type_;
  std::vector<uint8_t> data_;
};

} // namespace kuro

#endif
