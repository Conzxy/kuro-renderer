#include "kuro/util/endian.hh"
#include <stdio.h>
#include <gtest/gtest.h>

using namespace kuro;

TEST (endian_test, is_little_endian) {
  EXPECT_TRUE(is_little_endian());
}

TEST (endian_test, to_big_endian) {
  auto be = to_big_endian<int32_t>(1);
  EXPECT_TRUE(*&be != 1);
}
