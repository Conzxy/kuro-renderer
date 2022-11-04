#ifndef KURO_TGA_IMAGE_H__
#define KURO_TGA_IMAGE_H__

#include <stdint.h>
#include <vector>

namespace kuro {

// Force to align to 1
#pragma pack(push, 1)

/*
 * Header format:
 * |++++++++++++++++++++++++++++++|
 * | ID length                    | -- byte
 * | color map type               | -- byte
 * | image type                   | -- byte
 * |++++++++++++++++++++++++++++++|
 * | color map specification      | -- 5 bytes(following)
 * | 1. first_entry_index         | -- 2 bytes
 * | 2. color map length          | -- 2 bytes
 * | 3. color map entry_size      | -- bytes
 * |++++++++++++++++++++++++++++++|
 * | image specification          | -- 10 bytes(following)
 * | 1. x_origin                  | -- 2 bytes
 * | 2. y_origin                  | -- 2 bytes
 * | 3. image width               | -- 2 bytes
 * | 4. image length              | -- 2 bytes
 * | 5. pixel depth               | -- byte
 * | 6. image descriptor          | -- byte
 * |++++++++++++++++++++++++++++++|
 */
struct TgaHeader {
  uint8_t id_length = 0;
  uint8_t color_map_type = 0;
  uint8_t image_type = 0;
  uint16_t first_entry_index = 0;
  uint16_t color_map_length = 0;
  uint8_t color_map_entry_size = 0;
  uint16_t x_origin = 0;
  uint16_t y_origin = 0;
  uint16_t width = 0;
  uint16_t height = 0;
  uint8_t pixel_depth = 0; // aka. Bits per pixel(commonly, 8/16/24/32)

  /*
   * 1 bytes(8 bits):
   * |+++++++++++++++++++++++++++++++++++++++++++|
   * | 0 | 0 |       |      |    |     |    |    |
   * | unused| image origin | Alpha channel bits |
   * |+++++++++++++++++++++++++++++++++++++++++++|
   *
   * Alpha channel bits: The number of attribute bits per pixel(bbp)
   * image origin: The order in which pixel data is transferred from the
   *               file to the screen.
   *               (Bit4: left-to-right, aka. ->)
   *               (Bit5: top-to-bottom, aka. down)
   */
  uint8_t image_descriptor = 0;
};

struct TgaFooter {
  uint32_t ext_area_offset = 0;
  uint32_t dev_dir_offset = 0;
  char signature[16];
  char dot_ascii = '.';
  char terminator = '\0';
};

// Recover normal alignment requirement
#pragma pack(pop)

static_assert(sizeof(TgaHeader) == 18,
              "The length of TGA header is not required");

static_assert(sizeof(TgaFooter) == 26,
              "The length of TGA footer is not required");

struct TgaColor {
  static const TgaColor white;
  static const TgaColor red;

  TgaColor() = default;

  TgaColor(uint8_t _r, uint8_t _g, uint8_t _b, uint8_t _a = 255)
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

/**
 * TGA format image(*.tga)
 *
 * \see http://tfc.duke.free.fr/coding/tga_specs.pdf
 */
class TgaImage {
  // FIXME Shoule disable copy?

  /**
   * Don't care color-mapped
   */
  enum ImageType {
    NO_IMAGE_DATA = 0,
    GRAYSCALE = 1, // white to black
    RGB = 3,       // Red, Green, Blue channels
    RGBA = 4,      // Attribute(commonly, alpha channel)
  };

  enum ImageOriginOrder : uint8_t {
    BOTTOM_LEFT = 0x00,
    BOTTOM_RIGHT = 0x10,
    TOP_LEFT = 0x20,
    TOP_RIGHT = 0x30,
  };

  using Width = uint16_t;
  using Height = uint16_t;

 public:
  TgaImage(int w, int h, ImageType t = RGB);
  ~TgaImage() noexcept;

  /*--------------------------------------------------*/
  /* File operation                                   */
  /*--------------------------------------------------*/

  bool WriteTo(char const *path,
               ImageOriginOrder order = ImageOriginOrder::BOTTOM_LEFT,
               bool rle = false) noexcept;
  bool ReadFrom(char const *path) noexcept;

  /*--------------------------------------------------*/
  /* Pixel manipulation                               */
  /*--------------------------------------------------*/

  void SetPixel(int x, int y, TgaColor const &color) noexcept;
  TgaColor GetPixel(int x, int y) noexcept;
  
  /*--------------------------------------------------*/
  /* Getter                                           */
  /*--------------------------------------------------*/

  int bytes_per_pixel() const noexcept { return (int)image_type_; }
  int bits_per_pixel() const noexcept { return bytes_per_pixel() << 3; }
  int width() const noexcept { return width_; }
  int height() const noexcept { return height_; }

 private:
  uint16_t width_;
  uint16_t height_;

  ImageType image_type_;

  std::vector<uint8_t> image_data_; // FIXME Not optimal
};

} // namespace kuro

#endif // KURO_TGA_IMAGE_H__
