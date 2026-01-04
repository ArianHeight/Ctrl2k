#pragma once
#include "basictypes.h"

#define ENUM_CLASS_GENERIC_DUAL_ARG_OP_GEN(name, op) \
inline name operator##op(const name& val1, const name& val2)\
{\
    return static_cast<name>(static_cast<std::underlying_type_t<name>>(val1) op static_cast<std::underlying_type_t<name>>(val2));\
}
#define ENUM_CLASS_GENERIC_DUAL_ARG_UNDERLYING_OP_GEN(name, op) \
inline name operator##op(const name& val1, const std::underlying_type_t<name>& val2)\
{\
    return static_cast<name>(static_cast<std::underlying_type_t<name>>(val1) op val2); \
}
#define ENUM_CLASS_GENERIC_DUAL_ARG_OP_EQ_GEN(name, op) \
inline name& operator##op##=(name& val, const name& val1)\
{\
    val = static_cast<name>(static_cast<std::underlying_type_t<name>>(val) op static_cast<std::underlying_type_t<name>>(val1));\
    return val;\
}
#define ENUM_CLASS_GENERIC_DUAL_ARG_UNDERLYING_OP_EQ_GEN(name, op) \
inline name& operator##op##=(name& val, const std::underlying_type_t<name>& val1)\
{\
    val = static_cast<name>(static_cast<std::underlying_type_t<name>>(val) op val1);\
    return val;\
}
#define ENUM_CLASS_GENERIC_SINGLE_ARG_OP_GEN(name, op) \
inline name operator##op(const name& val)\
{\
    return static_cast<name>(op##static_cast<std::underlying_type_t<name>>(val));\
}
#define ENUM_CLASS_GENERIC_COMPARE_UNDERLYING_OP_GEN(name, op) \
inline bool operator##op(const name& val1, const std::underlying_type_t<name>& val2)\
{\
    return static_cast<std::underlying_type_t<name>>(val1) op val2; \
}

// generates enum class operator overrides for bitwise operations
#define ENUM_CLASS_BITWISE_OP_GEN(name) \
ENUM_CLASS_GENERIC_SINGLE_ARG_OP_GEN(name, ~)\
ENUM_CLASS_GENERIC_DUAL_ARG_OP_GEN(name, |)\
ENUM_CLASS_GENERIC_DUAL_ARG_OP_GEN(name, &)\
ENUM_CLASS_GENERIC_DUAL_ARG_OP_GEN(name, ^)
// generates enum class operator overrides for bitwise operations with automatic type conversion from underlying type
#define ENUM_CLASS_BITWISE_UNDERLYING_OP_GEN(name) \
ENUM_CLASS_GENERIC_DUAL_ARG_UNDERLYING_OP_GEN(name, |)\
ENUM_CLASS_GENERIC_DUAL_ARG_UNDERLYING_OP_GEN(name, &)\
ENUM_CLASS_GENERIC_DUAL_ARG_UNDERLYING_OP_GEN(name, ^)

// generate enum class opeartor overrides for bitwise equal operations(ie. |=, &=, ^=)
#define ENUM_CLASS_BITWISE_OP_EQ_GEN(name) \
ENUM_CLASS_GENERIC_DUAL_ARG_OP_EQ_GEN(name, |)\
ENUM_CLASS_GENERIC_DUAL_ARG_OP_EQ_GEN(name, &)\
ENUM_CLASS_GENERIC_DUAL_ARG_OP_EQ_GEN(name, ^)
// generate enum class opeartor overrides for bitwise equal operations(ie. |=, &=, ^=) with auto type conversion from underlying
#define ENUM_CLASS_BITWISE_UNDERLYING_OP_EQ_GEN(name) \
ENUM_CLASS_GENERIC_DUAL_ARG_UNDERLYING_OP_EQ_GEN(name, |)\
ENUM_CLASS_GENERIC_DUAL_ARG_UNDERLYING_OP_EQ_GEN(name, &)\
ENUM_CLASS_GENERIC_DUAL_ARG_UNDERLYING_OP_EQ_GEN(name, ^)

// generates enum class operator overrides for bitshifting operations
#define ENUM_CLASS_BITSHIFT_OP_GEN(name) \
ENUM_CLASS_GENERIC_DUAL_ARG_UNDERLYING_OP_GEN(name, <<)\
ENUM_CLASS_GENERIC_DUAL_ARG_UNDERLYING_OP_GEN(name, >>)\

// generates enum class operator overrides for bitshift equal operations(ie. <<= and >>=)
#define ENUM_CLASS_BITSHIFT_OP_EQ_GEN(name) \
ENUM_CLASS_GENERIC_DUAL_ARG_UNDERLYING_OP_EQ_GEN(name, <<)\
ENUM_CLASS_GENERIC_DUAL_ARG_UNDERLYING_OP_EQ_GEN(name, >>)\

// generates enum class operator overrides for comparison operators with auto type conversion from underlying
#define ENUM_CLASS_COMPARE_UNDERLYING_OP_GEN(name) \
ENUM_CLASS_GENERIC_COMPARE_UNDERLYING_OP_GEN(name, ==)\
ENUM_CLASS_GENERIC_COMPARE_UNDERLYING_OP_GEN(name, !=)\
ENUM_CLASS_GENERIC_COMPARE_UNDERLYING_OP_GEN(name, <)\
ENUM_CLASS_GENERIC_COMPARE_UNDERLYING_OP_GEN(name, >)\
ENUM_CLASS_GENERIC_COMPARE_UNDERLYING_OP_GEN(name, >=)\
ENUM_CLASS_GENERIC_COMPARE_UNDERLYING_OP_GEN(name, <=)

// generate all the bit operations
#define ENUM_CLASS_OP_GEN(name) \
ENUM_CLASS_BITWISE_OP_GEN(name)\
ENUM_CLASS_BITSHIFT_OP_GEN(name)\
ENUM_CLASS_BITSHIFT_OP_EQ_GEN(name)\
ENUM_CLASS_BITWISE_OP_EQ_GEN(name)

// generate all the operations with auto type conversion from the underlying type
#define ENUM_CLASS_UNDERLYING_OP_GEN(name) \
ENUM_CLASS_BITWISE_UNDERLYING_OP_GEN(name)\
ENUM_CLASS_BITWISE_UNDERLYING_OP_EQ_GEN(name)\
ENUM_CLASS_COMPARE_UNDERLYING_OP_GEN(name)

#define ENUM_CLASS_FULL_OP_GEN(name) \
ENUM_CLASS_OP_GEN(name)\
ENUM_CLASS_UNDERLYING_OP_GEN(name)

#define ENUM_ENTRY_RAW(x) x,
#define ENUM_ENTRY_STR(x) #x,

/*
Generates an enum map(an enum class and a c-string map that can be indexed with the enum) using a def macro.
Example:
#define ENUM_COLOURS(ENUM_ENTRY)\
    ENUM_ENTRY(RED)\
    ENUM_ENTRY(BLUE)\
    ENUM_ENTRY(GREEN)
ENUM_MAP_GEN(Colours, uint16_t, ENUM_COLOURS);
#undef ENUM_COLOURS
*/
#define ENUM_MAP_GEN(name, type, def) \
    enum class name : type { def(ENUM_ENTRY_RAW) SIZE };\
    const c_string name##StrMap[] = { def(ENUM_ENTRY_STR) };

#define ENUM_CLASS_CONVERT(name, underlying) static_cast<name>(underlying)
#define ENUM_CLASS_UNDERLYING(name, val) static_cast<std::underlying_type_t<name>>(name##::##val)
#define ENUM_CLASS_INST_UNDERLYING(name, inst) static_cast<std::underlying_type_t<name>>(inst)

#define ENUM_CLASS_SET(inst, name, underlying) inst = ENUM_CLASS_CONVERT(name, underlying)
#define ENUM_CLASS_COMPARE(name, val, op, underlying) name##::##val op ENUM_CLASS_CONVERT(name, underlying)

#define ENUM_MAP_STRING(name, val) name##StrMap[ENUM_CLASS_UNDERLYING(name, val)]
#define ENUM_MAP_INST_STRING(name, inst) name##StrMap[ENUM_CLASS_INST_UNDERLYING(name, inst)]

template<typename T, typename V>
inline bool FlagAnySet(const T& flag, const V& val)
{
    return (flag & val) != 0;
}

template<typename T, typename V>
inline bool FlagAllSet(const T& flag, const V& val)
{
    return (flag & val) == val;
}
