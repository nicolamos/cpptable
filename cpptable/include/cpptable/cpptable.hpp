#pragma once
#ifndef CPPTABLE_HPP
#define CPPTABLE_HPP

#include <concepts>
#include <utility>
#include <initializer_list>
#include <vector>
#include <string>
#include <string_view>
#include <ranges>


namespace tbl
{

template <typename T>
struct column_info
{
    using value_type = T;
    constexpr column_info() = default;
    template <std::convertible_to<std::string> NameT>
    constexpr column_info(NameT name) : name{std::move(name)} {}
    std::string name;
};


template <typename... Ts>
struct default_header
{
    using row_type = std::tuple<Ts...>;
    using column_tuple = std::tuple<column_info<Ts>...>;

    constexpr default_header() = default;
    constexpr default_header(column_info<Ts>... cinfo) : columns{std::move(cinfo)...} {}

    constexpr auto names() const {
        return std::apply([](const auto&... args) { return std::array<std::string, sizeof...(args)>{args.name...}; }, columns);
    }

    template <std::size_t I>
    struct column { using type = typename std::tuple_element_t<I, column_tuple>::value_type; };
    template <std::size_t I>
    using column_t = typename column<I>::type;

    column_tuple columns;
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
    using container_type = ContainerT;
    using reference = typename container_type::reference;
    using iterator = typename container_type::iterator;
    using const_iterator = typename container_type::const_iterator;

    header_type header;

    // template <std::convertible_to<std::string> ...Ts>
    // constexpr basic_table(Ts&& ...names) : header{{std::string(std::forward<Ts>(names))}...} {}
    constexpr basic_table(std::initializer_list<row_type> init) : rows_{std::move(init)} {}

    constexpr const_iterator begin() const { return rows_.begin(); }
    constexpr iterator begin() { return rows_.begin(); }
    constexpr const_iterator end() const { return rows_.end(); }
    constexpr iterator end() { return rows_.end(); }

    template<typename... Args>
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

#endif // CPPTABLE_HPP
