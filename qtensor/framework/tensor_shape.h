#ifndef QTENSOR_FRAMEWORK_TENSOR_SHAPE_H_
#define QTENSOR_FRAMEWORK_TENSOR_SHAPE_H_

#include "qtensor/core/int.h"
#include "qtensor/core/status.h"

#include <vector>
#include <memory>
#include <cstring>
#include <initializer_list>

namespace qtensor {

class TensorShape {
 public:
  TensorShape()
    : size_(0), dims_(nullptr) {}
  TensorShape(const std::vector<uint64_t>& dims)
    : size_(dims.size()), dims_(new uint64_t[size_]) {
    std::memcpy(dims_.get(), &dims_[0], sizeof(uint64_t) * size_);
  }
  TensorShape(const std::initializer_list<uint64_t>& dims)
    : size_(dims.size()), dims_(new uint64_t[size_]) {
    auto iter = dims.begin();
    for (uint64_t i = 0; i < size_; i++) {
      dims_[i] = *iter++;
    }
  }
  TensorShape(const TensorShape& rhs)
    : size_(rhs.size_), dims_(new uint64_t[size_]) {
    std::memcpy(dims_.get(), rhs.dims_.get(), sizeof(uint64_t) * size_);
  }
  TensorShape(TensorShape&& rhs)
    : size_(rhs.size_), dims_(std::move(rhs.dims_)) {}
  TensorShape& operator=(const TensorShape& rhs) {
    size_ = rhs.size_;
    dims_.reset(new uint64_t[size_]);
    std::memcpy(dims_.get(), rhs.dims_.get(), sizeof(uint64_t) * size_);
    return *this;
  }
  TensorShape& operator=(TensorShape&& rhs) {
    size_ = rhs.size_;
    dims_ = std::move(rhs.dims_);
    return *this;
  }

  uint64_t& operator[](uint64_t id) {
    return dims_[id];
  }
  uint64_t operator[](uint64_t id) const {
    return dims_[id];
  }

  uint64_t NumElements() const {
    uint64_t ret = 1;
    for (uint64_t i = 0; i < size_; i++) {
      ret *= dims_[i];
    }
    return ret;
  }
 private:
  uint64_t size_;
  std::unique_ptr<uint64_t[]> dims_;
};

}

#endif

