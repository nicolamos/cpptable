#pragma once
#ifndef CPPTABLE_HPP
#define CPPTABLE_HPP

#include <concepts>
#include <utility>
#include <algorithm>
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


template <std::size_t... Is>
constexpr auto to_string_tuple(std::index_sequence<Is...>)
{
    return std::make_tuple(std::to_string(Is)...);
}


template <typename... Ts>
constexpr auto to_string_tuple()
{
    return to_string_tuple(std::index_sequence_for<Ts...>{});
}


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

    column_tuple columns{to_string_tuple<Ts...>()};
};


template <typename ...Ts>
constexpr auto make_header(column_info<Ts>&&... cinfo)
{
    return default_header<Ts...>(std::forward<column_info<Ts>>(cinfo)...);
}


template <
    typename HeaderT,
    typename ContainerT = std::vector<typename HeaderT::row_type>
>
class basic_table
{
public:
    using header_type = HeaderT;
    using row_type = typename header_type::row_type;
    using column_tuple = typename header_type::column_tuple;
    using container_type = ContainerT;
    using size_type = typename container_type::size_type;
    using reference = typename container_type::reference;
    using iterator = typename container_type::iterator;
    using const_iterator = typename container_type::const_iterator;

    header_type header;

    constexpr basic_table() = default;
    constexpr basic_table(std::initializer_list<row_type> init) : rows_{std::move(init)} {}
    template <std::convertible_to<std::string> ...Names>
    constexpr basic_table(Names&& ...names) : header{{std::string(std::forward<Names>(names))}...} {}
    template <typename ...Ts>
    constexpr basic_table(column_info<Ts> const&... cinfo) : header{cinfo...} {}

    constexpr const_iterator begin() const { return rows_.begin(); }
    constexpr iterator begin() { return rows_.begin(); }
    constexpr const_iterator end() const { return rows_.end(); }
    constexpr iterator end() { return rows_.end(); }

    constexpr auto capacity() const { return rows_.capacity(); }
    constexpr auto size() const { return rows_.size(); }
    constexpr void reserve(size_type new_cap) { rows_.reserve(new_cap); }

    template<typename... Args>
    constexpr reference emplace_back(Args&&... args) { return rows_.emplace_back(std::forward<Args>(args)...); }

    template <typename T>
    constexpr void push_back(T&& value) { rows_.push_back(std::forward<T>(value)); }

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
    using typename super::column_tuple;
    using super::super;
};


template <typename ...Ts, std::convertible_to<std::string> ...Names>
constexpr auto make_table(Names&& ...names) -> table<Ts...>
{
    return {names...};
}


template <typename... Ts>
constexpr auto make_table(column_info<Ts> const& ...cinfo) -> table<Ts...>
{
    return {cinfo...};
}


template <typename ...Tables>
auto table_join(Tables&& ...tables)
{
    using std::views::zip;

    auto columns = std::tuple_cat(tables.header.columns...);
    auto joined_table = std::apply([](auto const&... args) { return table<typename std::decay_t<decltype(args)>::value_type...>{args...}; }, columns);

    joined_table.reserve(std::min({tables.size()...}));

    for (const auto& values : zip(tables...)) {
        auto new_row = std::apply([](const auto&... args) { return std::tuple_cat(args...); }, values);
        joined_table.push_back(new_row);
    }

    return joined_table;
}

} // namespace tbl

#endif // CPPTABLE_HPP
