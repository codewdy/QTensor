#include "qtensor/core/status.h"

namespace qtensor {

const std::string Status::kEmptyString = "";
const std::unordered_map<int64_t, std::string> Status::kCodeEnumString = {
  {Status::kOk, "Ok"},
  {Status::kArgumentError, "ArgumentError"},
  {Status::kIndexOverflow, "IndexOverflow"},
};

}
