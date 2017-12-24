#ifndef QTENSOR_FRAMEWORK_OPERATOR_H_
#define QTENSOR_FRAMEWORK_OPERATOR_H_

#include <unordered_map>

#include "qtensor/core/core.h"
#include "qtensor/framework/tensor.h"

namespace qtensor {

class Operator {
 public:

  struct InitContext {
    std::unordered_map<std::string, Any> params;
  };

  struct Context {
    std::vector<Tensor> inputs;
    std::vector<Tensor> outputs;
    size_t threads;
    Any op_ctx;
  };

  virtual ~Operator() {}
  virtual Status Init(InitContext* ctx) = 0;
  virtual Status BeforeRun(Context* ctx) const = 0;
  virtual Status Run(Context* ctx, size_t thread_id) const = 0;
  virtual Status AfterRun(Context* ctx) const = 0;
};

}

#endif

