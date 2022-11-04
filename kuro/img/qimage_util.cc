#include "qimage_util.hh"

namespace kuro {

QImage::Format FrameImageTypeToQImageFormat(FrameBuffer::ImageType type)
{
  switch (type) {
    case FrameBuffer::IMAGE_TYPE_RGB:
      return QImage::Format_RGB32;
    case FrameBuffer::IMAGE_TYPE_RGBA:
      return QImage::Format_ARGB32;
  }

  return QImage::Format_Invalid;
}

QImage FrameBufferToQImage(FrameBuffer const &frame_buffer)
{
  QImage image(frame_buffer.GetRawData(), frame_buffer.GetWidth(),
               frame_buffer.GetHeight(),
               FrameImageTypeToQImageFormat(frame_buffer.GetImageType()));
  return image;
}

} // namespace kuro
