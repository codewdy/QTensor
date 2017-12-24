#include "gtest/gtest.h"
#include "qtensor/framework/tensor.h"

using qtensor::Tensor;
using qtensor::TensorShape;
using qtensor::DataType;

TEST(Tensor, Tensor) {
  Tensor a;
  Tensor b(TensorShape({1, 2}), DataType::kInt16);
  Tensor c(b);
  Tensor d(a);
  ASSERT_FALSE(a.IsInitialized());
  ASSERT_TRUE(b.IsInitialized());
  ASSERT_TRUE(c.IsInitialized());
  ASSERT_FALSE(d.IsInitialized());
  b.Raw<int16_t>()[0] = 3;
  b.Raw<int16_t>()[1] = 19;
  ASSERT_EQ(3, c.Raw<int16_t>()[0]);
  ASSERT_EQ(19, c.Raw<int16_t>()[1]);
}

