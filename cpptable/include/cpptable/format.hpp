#pragma once
#ifndef CPPTABLE_FORMAT_HPP
#define CPPTABLE_FORMAT_HPP

#include <fmt/format.h>
#include "cpptable.hpp"

template <typename T>
struct fmt::formatter<tbl::column_info<T>> : fmt::formatter<std::string_view>
{
    using column_type = tbl::column_info<T>;
    auto format(const column_type& cinfo, format_context& ctx) const
    {
        return fmt::formatter<std::string_view>::format(cinfo.name, ctx);
    }
};

#endif // CPPTABLE_FORMAT_HPP
