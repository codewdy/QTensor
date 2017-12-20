#include "gtest/gtest.h"
#include "qtensor/core/status.h"

using qtensor::Status;

TEST(Status, Constructor) {
  Status a;
  Status b(Status::kArgumentError, "error1");
  Status c = Status::ArgumentError("error2");
  ASSERT_EQ("OK", a.ToString());
  ASSERT_EQ("ArgumentError: error1", b.ToString());
  ASSERT_EQ("ArgumentError: error2", c.ToString());
}

TEST(Status, CopyAndMove) {
  Status st, err = Status::ArgumentError("error");
  Status x0(st);
  ASSERT_EQ("OK", x0.ToString());
  Status x1(Status::Ok());
  ASSERT_EQ("OK", x1.ToString());
  Status x2 = st;
  ASSERT_EQ("OK", x2.ToString());
  Status x3 = Status();
  ASSERT_EQ("OK", x3.ToString());

  Status x4(err);
  ASSERT_EQ("ArgumentError: error", x4.ToString());
  Status x5(Status::ArgumentError("error"));
  ASSERT_EQ("ArgumentError: error", x5.ToString());
  Status x6 = err;
  ASSERT_EQ("ArgumentError: error", x6.ToString());
  Status x7 = Status::ArgumentError("error");
  ASSERT_EQ("ArgumentError: error", x7.ToString());
}

TEST(Status, IsOk) {
  Status st;
  st = Status::Ok();
  ASSERT_TRUE(st.IsOk());
  st = Status::ArgumentError("error");
  ASSERT_FALSE(st.IsOk());
}

TEST(Status, Errors) {
  Status st;
  st = Status::Ok();
  ASSERT_EQ("OK", st.ToString());
  st = Status::ArgumentError("error");
  ASSERT_EQ("ArgumentError: error", st.ToString());
  st = Status::IndexOverflow("error");
  ASSERT_EQ("IndexOverflow: error", st.ToString());
}

