#ifndef QTENSOR_FRAMEWORK_TYPES_H_
#define QTENSOR_FRAMEWORK_TYPES_H_

#include "qtensor/core/int.h"

namespace qtensor {

namespace types {

#define QTENSOR_TYPE_XMACRO \
  QTENSOR_TYPE_X(kInt8, int8_t)          \
  QTENSOR_TYPE_X(kInt16, int16_t)        \
  QTENSOR_TYPE_X(kInt32, int32_t)        \
  QTENSOR_TYPE_X(kInt64, int64_t)        \
  QTENSOR_TYPE_X(kFloat, float)          \
  QTENSOR_TYPE_X(kDouble, double)

enum DataType {
#define QTENSOR_TYPE_X(ID, TYPE) ID,
  QTENSOR_TYPE_XMACRO
#undef QTENSOR_TYPE_X
};


template<DataType type> struct EnumToType { };
#define QTENSOR_TYPE_X(ID, TYPE) \
template<> struct EnumToType<ID> { using Type = TYPE; };
QTENSOR_TYPE_XMACRO
#undef QTENSOR_TYPE_X


template<typename T> struct TypeToEnum { };
#define QTENSOR_TYPE_X(ID, TYPE) \
template<> struct TypeToEnum<TYPE> { static const DataType type = ID; };
QTENSOR_TYPE_XMACRO
#undef QTENSOR_TYPE_X


uint64_t SizeOfType(DataType type) {
  switch (type) {
#define QTENSOR_TYPE_X(ID, TYPE) \
    case ID: return sizeof(TYPE);
    QTENSOR_TYPE_XMACRO
#undef QTENSOR_TYPE_X
  }
}


#undef QTENSOR_TYPE_XMACRO

}

// enum DataType { ... }
using types::DataType;
// template<DataType type> struct EnumToType { using Type = ...; };
using types::EnumToType;
// template<typename T> struct TypeToEnum { static const DataType type; };
using types::TypeToEnum;
// uint64_t SizeOfType(DataType type)
using types::SizeOfType;

}

#endif

