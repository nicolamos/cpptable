#pragma once
#ifndef CPPTABLE_FORMAT_HPP
#define CPPTABLE_FORMAT_HPP

#include <optional>
#include <fmt/format.h>
#include "cpptable.hpp"
#include "nullable.hpp"
#include "utils.hpp"

namespace tbl
{

template <typename... Ts>
constexpr auto format_as(const default_header<Ts...>& header) { return header.columns; }

} // namespace tbl


template <typename T>
struct fmt::formatter<tbl::column_info<T>> : fmt::formatter<std::string_view>
{
    using column_type = tbl::column_info<T>;
    auto format(const column_type& cinfo, format_context& ctx) const
    {
        return fmt::formatter<std::string_view>::format(cinfo.name, ctx);
    }
};


template <typename T>
struct fmt::formatter<tbl::formattable_optional<T>>
{
    using formattable_optional = tbl::formattable_optional<T>;
    using value_type = typename formattable_optional::value_type;

    std::string_view underlying_fmt;
    std::string_view or_else;

    constexpr auto parse(format_parse_context& ctx) -> decltype(ctx.begin())
    {
        // {:<if present{}><if not present>}
        auto it = ctx.begin(), end = ctx.end();
        auto get_marker = [&it, end]() constexpr -> std::string_view {
            if (it == end || *it != '<') return std::string_view(nullptr, 0); // no match
            auto start = ++it;

            while (it != end && (*it++ != '>'));
            if (it == end) throw fmt::format_error("invalid format, unfinished range");

            return std::string_view(start, it - 1);
            };

        underlying_fmt = "{}";
        or_else = "missing";

        auto first = get_marker();
        if (first.data() != nullptr) underlying_fmt = first;

        auto second = get_marker();
        if (second.data() != nullptr) or_else = second;

        // Check if reached the end of the range:
        if (it != end && *it != '}') throw fmt::format_error("invalid format, no end bracket");
        return it;
    }

    template <typename FormatContext>
    auto format(const formattable_optional& p, FormatContext& ctx) const -> decltype(ctx.out())
    {
        if (p) {
            return vformat_to(ctx.out(), underlying_fmt, format_arg_store<FormatContext, value_type>{*p});
        }
        else {
            return fmt::format_to(ctx.out(), "{}", or_else);
        }
    }
};


template <typename R>
struct fmt::formatter<tbl::row_record<R>> : public fmt::ostream_formatter {};

#endif // CPPTABLE_FORMAT_HPP
