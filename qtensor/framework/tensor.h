#ifndef QTENSOR_FRAMEWORK_TENSOR_H_
#define QTENSOR_FRAMEWORK_TENSOR_H_

#include "qtensor/framework/tensor_shape.h"
#include "qtensor/framework/types.h"

#include <memory>
#include <atomic>

namespace qtensor {

class Tensor {
 public:
  Tensor()
    : state_(nullptr) {
  }

  ~Tensor() {
    Unref();
  }

  Tensor(const TensorShape& shape, DataType type)
    : state_(new State(shape, type)) {
  }

  Tensor(const Tensor& rhs)
    : state_(rhs.state_) {
    Ref();
  }

  Tensor(Tensor&& rhs)
    : state_(rhs.state_) {
    rhs.state_ = nullptr;
  }

  Tensor& operator=(const Tensor& rhs) {
    Unref();
    state_ = rhs.state_;
    Ref();
    return *this;
  }

  Tensor& operator=(Tensor&& rhs) {
    std::swap(state_, rhs.state_);
    return *this;
  }

  bool IsInitialized() {
    return state_ != nullptr;
  }

  template<typename T>
  T* Raw() const {
    return reinterpret_cast<T*>(state_->buffer.get());
  }
 private:
  void Ref() {
    if (state_ == nullptr) {
      return;
    }
    ++state_->ref;
  }

  void Unref() {
    if (state_ == nullptr) {
      return;
    }
    if (--state_->ref == 0) {
      delete state_;
    }
  }

  struct State {
    TensorShape shape;
    DataType type;
    std::unique_ptr<int8_t[]> buffer;
    std::atomic<uint64_t> ref;
    State(const TensorShape& shape_, DataType type_)
      : shape(shape_), type(type_),
        buffer(new int8_t[SizeOfType(type_) * shape_.NumElements()]),
        ref(1) {}
  };
  State* state_;
};

}

#endif

