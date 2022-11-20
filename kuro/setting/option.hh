#ifndef KURO_SETTINGS_OPTION_H__
#define KURO_SETTINGS_OPTION_H__

#include <string>
#include <vector>

namespace kuro {

struct Option {
  std::vector<std::string> models;
};

inline Option &kuro_option()
{
  static Option option;
  return option;
}

} // namespace kuro

#endif
