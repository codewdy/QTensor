#include "gtest/gtest.h"
#include "qtensor/core/any.h"

using qtensor::Any;

namespace {

class MockData {
 public:
  MockData() : k(0) {}
  MockData(MockData&&) : k(1) {}
  MockData(const MockData&) : k(2) {}
  int k;
};

}

TEST(Any, Any) {
  Any x, y;
  MockData d;
  x.Set<MockData>(d);
  y.Set<MockData>(std::move(d));
  ASSERT_EQ(2, x.Get<MockData>()->k);
  ASSERT_EQ(1, y.Get<MockData>()->k);
  ASSERT_EQ(nullptr, x.Get<int>());
  x.Set<int>(10);
  ASSERT_EQ(10, *x.Get<int>());
}

