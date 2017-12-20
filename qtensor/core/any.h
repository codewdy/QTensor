#ifndef QTENSOR_CORE_ANY_
#define QTENSOR_CORE_ANY_

#include <memory>

namespace qtensor {

class Any {
 public:
  template<typename T, typename... Targs>
  void Set(Targs&&... args) {
    internal_.reset(new AnyPointerTyped<T>(std::forward<Targs>(args)...));
  }
  template<typename T>
  T* Get() const {
    AnyPointerTyped<T>* raw_internal_ =
      dynamic_cast<AnyPointerTyped<T>*>(internal_.get());
    if (raw_internal_) {
      return &(raw_internal_->internal);
    } else {
      return nullptr;
    }
  }
 private:
  class AnyPointer {
   public:
    virtual ~AnyPointer() {}
  };
  template<typename T>
  class AnyPointerTyped : public AnyPointer {
   public:
    template<typename... Targs>
    AnyPointerTyped(Targs&&... args)
      : internal(std::forward<Targs>(args)...) {}
    T internal;
  };
  std::unique_ptr<AnyPointer> internal_;
};

}

#endif

