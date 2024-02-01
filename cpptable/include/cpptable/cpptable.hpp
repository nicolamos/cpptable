#pragma once
#ifndef CPPTABLE_HPP
#define CPPTABLE_HPP

#include <concepts>
#include <utility>
#include <vector>
#include <string>
#include <string_view>
#include <ranges>
#include <fmt/format.h>


namespace tbl
{

template <typename T>
struct position
{
    T x;
    T y;
    T z;
};


template <typename T>
struct column_info
{
    using value_type = T;
    std::string name;
};


template <typename... Ts>
struct default_header
{
    using row_type = std::tuple<Ts...>;
    using column_type = std::tuple<column_info<Ts>...>;

    constexpr default_header(column_info<Ts>... cinfo) : columns{cinfo...} {}

    constexpr auto names() const {
        return std::apply([](const auto&... args) { return std::array<std::string, sizeof...(args)>{args.name...}; }, columns);
    }

    column_type columns;
};


template <typename... Ts>
constexpr auto format_as(const default_header<Ts...>& header) { return header.columns; }


template <
    typename HeaderT,
    typename ContainerT = std::vector<typename HeaderT::row_type>
>
class basic_table
{
public:
    using header_type = HeaderT;
    using row_type = typename header_type::row_type;
    using column_type = typename header_type::column_type;
    using container_type = ContainerT;
    using reference = typename container_type::reference;
    using iterator = typename container_type::iterator;
    using const_iterator = typename container_type::const_iterator;

    header_type header;

    template <std::convertible_to<std::string> ...Ts>
    constexpr basic_table(Ts&& ...names) : header{{std::string(std::forward<Ts>(names))}...} {}

    constexpr const_iterator begin() const { return rows_.begin(); }
    constexpr iterator begin() { return rows_.begin(); }
    constexpr const_iterator end() const { return rows_.end(); }
    constexpr iterator end() { return rows_.end(); }

    template< class... Args >
    constexpr reference emplace_back(Args&&... args) { return rows_.emplace_back(std::forward<Args>(args)...); }

    constexpr auto names() const { return header.names(); }

private:
    container_type rows_;
};


template <typename... Ts>
struct table : public basic_table<default_header<Ts...>>
{
    using super = basic_table<default_header<Ts...>>;
    using typename super::header_type;
    using typename super::row_type;
    using super::super;
};

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

#endif // CPPTABLE_HPP
