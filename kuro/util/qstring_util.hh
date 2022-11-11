#ifndef KURO_UTIL_QSTRING_UTIL_H__
#define KURO_UTIL_QSTRING_UTIL_H__

#include <QString>

namespace kuro {

inline char const *QStringToCString(QString const &str)
{
  QByteArray ba = str.toLocal8Bit();
  return ba.data();
}

} // namespace kuro

#endif
