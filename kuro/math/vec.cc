#include "vec.hh"

using namespace kuro;

#define VEC_TEMPLATE_PARAMS template <typename T, size_t N>
#define VEC_CLASS_TEMPLATE Vec<T, N>

namespace kuro {

template class Vec<int, 2>;
template class Vec<int, 3>;
template class Vec<int, 4>;
template class Vec<float, 2>;
template class Vec<float, 3>;
template class Vec<float, 4>;

} // namespace kuro
