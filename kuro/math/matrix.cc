#include "matrix.hh"

using namespace kuro;

#define MATRIX_TEMPLATE_PARAMS template <typename T, size_t R, size_t C>
#define MATRIX_CLASS_TEMPLATE Matrix<T, R, C>

MATRIX_TEMPLATE_PARAMS
MATRIX_CLASS_TEMPLATE::Matrix() noexcept
{

}

MATRIX_TEMPLATE_PARAMS
MATRIX_CLASS_TEMPLATE::Matrix(T const &init) noexcept
{
  for (auto & row : data_)
    row.fill(init);
}

MATRIX_TEMPLATE_PARAMS
MATRIX_CLASS_TEMPLATE::~Matrix() noexcept = default;

