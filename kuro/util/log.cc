#include "log.hh"

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

namespace kuro {

void DebugPrintf(char const *fmt, ...)
{
  char const *debug_value = getenv("KURO_DEBUG"); 
  if (!debug_value || strcmp(debug_value, "1") != 0) {
    return;
  }

  va_list ap;
  va_start(ap, fmt);
  vprintf(fmt, ap);
  va_end(ap);
}

} // namespace kuro
