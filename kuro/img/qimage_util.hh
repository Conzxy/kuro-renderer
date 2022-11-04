#ifndef KURO_IMG_QIMAGE_UTIL_H__
#define KURO_IMG_QIMAGE_UTIL_H__

#include <QImage>

#include "frame_buffer.hh"

namespace kuro {

static_assert(std::is_same<uchar, uint8_t>::value, "uchar should be same with uint8_t");

QImage::Format FrameImageTypeToQImageFormat(FrameBuffer::ImageType type);
QImage FrameBufferToQImage(FrameBuffer const &frame_buffer);

} // namespace kuro

#endif
