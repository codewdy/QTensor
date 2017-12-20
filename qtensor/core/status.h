#ifndef QTENSOR_CORE_STATUS_
#define QTENSOR_CORE_STATUS_

#include <memory>
#include <string>
#include <unordered_map>

namespace qtensor {

class Status {
 public:
  enum CodeEnum {
    kOk,
    kArgumentError,
    kIndexOverflow
  };
  Status() {}

  Status(CodeEnum code, const std::string& msg)
    : state_(new State{.code=code, .msg=msg}) {}

  Status(const Status& rhs)
    : state_(rhs.state_ == nullptr ? nullptr : new State(*rhs.state_)) {}

  Status(Status&& rhs)
    : state_(std::move(rhs.state_)) {}

  Status& operator=(const Status& rhs) {
    state_.reset(rhs.state_ == nullptr ? nullptr : new State(*rhs.state_));
    return *this;
  }
  Status& operator=(Status&& rhs) {
    state_ = std::move(rhs.state_);
    return *this;
  }

  bool IsOk() const { return state_ == nullptr; }

  const std::string& Msg() const {
    return state_ == nullptr ? kEmptyString : state_->msg;
  }

  CodeEnum Code() const {
    return state_ == nullptr ? kOk : state_->code;
  }

  std::string ToString() const {
    return state_ == nullptr ? ("OK") :
      (CodeEnumToString(state_->code) + ": " + state_->msg);
  }

  static Status Ok() {
    return Status();
  }

  static Status ArgumentError(const std::string& msg) {
    return Status(kArgumentError, msg);
  }

  static Status IndexOverflow(const std::string& msg) {
    return Status(kIndexOverflow, msg);
  }

 private:
  struct State {
    CodeEnum code;
    std::string msg;
  };
  std::unique_ptr<State> state_;

  static std::string CodeEnumToString(CodeEnum code) {
    auto iter = kCodeEnumString.find(code);
    return iter == kCodeEnumString.end() ? "UnknownTypeError" : iter->second;
  }
  static const std::string kEmptyString;
  static const std::unordered_map<int64_t, std::string> kCodeEnumString;
};

}

#endif

