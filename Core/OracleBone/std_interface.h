#pragma once
#include "simple_string.h"
#include "borrowed_string.h"

#include <ostream>

namespace obn
{

template <typename chartype>
inline std::basic_ostream<chartype>& operator<<(std::basic_ostream<chartype>& os, const borrowed_string<chartype>& op1)
{
    os.write(op1.c_str(), op1.length());
    return os;
}

template <typename chartype, template <typename U, size_t N> typename datatype, size_t num, bool dyn>
inline std::basic_ostream<chartype>& operator<<(std::basic_ostream<chartype>& os, const simple_string<datatype, chartype, num, dyn>& op1)
{
    os.write(op1.c_str(), op1.length());
    return os;
}

}