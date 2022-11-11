#ifndef KURO_SETTINGS_OPTION_H__
#define KURO_SETTINGS_OPTION_H__

#include <string>

namespace kuro {

struct Option {
  std::string model_name = "";
};

inline Option &kuro_option()
{
  static Option option;
  return option;
}

} // namespace kuro

#endif
