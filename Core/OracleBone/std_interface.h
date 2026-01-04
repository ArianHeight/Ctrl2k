#pragma once
#include "simple_string.h"
#include "borrowed_string.h"

#include <ostream>
#include <format>

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

template <typename chartype>
struct std::formatter<obn::borrowed_string<chartype>> : std::formatter<std::basic_string_view<chartype>>
{
    auto format(const obn::borrowed_string<chartype>& view, std::format_context& ctx) const
    {
        return std::formatter<std::basic_string_view<chartype>>::format({ view.c_str(), view.length() }, ctx);
    }
};

template <typename chartype, template <typename U, size_t N> typename datatype, size_t num, bool dyn>
struct std::formatter<obn::simple_string<datatype, chartype, num, dyn>> : std::formatter<std::basic_string_view<chartype>>
{
    auto format(const obn::simple_string<datatype, chartype, num, dyn>& str, std::format_context& ctx) const
    {
        const auto view = str.view();
        return std::formatter<std::basic_string_view<chartype>>::format({ view.c_str(), view.length() }, ctx);
    }
};
