#pragma once
#ifndef CPPTABLE_HPP
#define CPPTABLE_HPP

#include <concepts>
#include <type_traits>
#include <utility>
#include <initializer_list>
#include <string>
#include <vector>
#include <ranges>
#include <algorithm>


namespace tbl
{

namespace detail
{

template <std::size_t ...Is>
constexpr auto to_string_tuple(std::index_sequence<Is...>)
{
    return std::make_tuple(std::to_string(Is)...);
}


template <typename ...Ts>
constexpr auto to_string_tuple()
{
    return to_string_tuple(std::index_sequence_for<Ts...>{});
}

} // namespace detail


template <typename RowT> struct row_info;
template <typename RowT> struct columns_type_info;
template <typename RecordT> struct row_record;
template <typename RecordT, typename ...Ts> struct row_record_tuple;
template <typename RecordT, typename ...Ts> struct row_info_record;


template <typename T, typename StringT = std::string>
class column_info
{
public:
    using value_type = T;
    using string_type = StringT;

    constexpr column_info() = default;

    template <std::convertible_to<string_type> NameT>
    constexpr column_info(NameT&& name) : name{std::forward<NameT>(name)} {}

    string_type name;
};


template <typename ...Ts>
struct row_info<std::tuple<Ts...>>
{
    using tuple_type = std::tuple<Ts...>;
    using row_type = tuple_type;
    using column_tuple = std::tuple<column_info<Ts>...>;
    template <std::size_t I>
    struct column { using type = typename std::tuple_element_t<I, column_tuple>::value_type; };
    template <std::size_t I>
    using column_t = typename column<I>::type;
    static constexpr auto size() { return std::tuple_size_v<row_type>; }
};


template <typename RecordT, typename ...Ts>
struct row_info<row_record_tuple<RecordT, Ts...>> : public row_info_record<RecordT, Ts...> {};


template <typename ...Ts>
using row_info_tuple = row_info<std::tuple<Ts...>>;


template <typename RowT>
struct columns_type_info : public row_info<RowT> {};


template <typename ...Ts>
struct columns_type_info<std::tuple<Ts...>> : public row_info_tuple<Ts...> {};


template <typename ...Ts>
using columns_type_info_tuple = columns_type_info<std::tuple<Ts...>>;


template <typename RecordT, typename ...Ts>
using columns_type_info_record = columns_type_info<row_record_tuple<RecordT, Ts...>>;


template <typename RecordT, typename ...Ts>
struct row_info_record : public row_info_tuple<Ts...>
{
    using row_type = RecordT;
};


template <typename ...Ts>
struct default_header : public columns_type_info_tuple<Ts...>
{
    using super = columns_type_info_tuple<Ts...>;
    using columns_info = super;
    using typename super::column_tuple;

    constexpr default_header() = default;
    constexpr default_header(column_info<Ts> ...cinfo) : columns{std::move(cinfo)...} {}

    constexpr auto names() const {
        return std::apply([](auto const& ...args) { return std::array<std::string, sizeof...(args)>{args.name...}; }, columns);
    }

    column_tuple columns{detail::to_string_tuple<Ts...>()};
};


template <typename RecordT, typename ...Ts>
class record_header : public columns_type_info_record<RecordT, Ts...>
{
public:
    using super = columns_type_info_record<RecordT, Ts...>;
    using columns_info = super;
    using typename super::column_tuple;

    constexpr record_header() = default;
    template <typename ...Args>
    constexpr record_header(Args&& ...args) : columns{std::move(args)...} {}

    constexpr auto names() const {
        return std::apply([](auto const& ...args) { return std::array<std::string, sizeof...(args)>{args.name...}; }, columns);
    }

    column_tuple columns{};
};


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
    using size_type = typename container_type::size_type;
    using reference = typename container_type::reference;
    using const_reference = typename container_type::const_reference;
    using iterator = typename container_type::iterator;
    using const_iterator = typename container_type::const_iterator;

    header_type header;

    constexpr basic_table() = default;
    constexpr basic_table(std::initializer_list<row_type> init) : rows_{std::move(init)} {}
    // template <std::convertible_to<std::string> ...Names>
    // constexpr basic_table(Names&&... names) : header{{std::string(std::forward<Names>(names))}...} {}
    template <typename H = header_type, typename C = container_type>
        requires (std::convertible_to<H, header_type> && std::convertible_to<C, container_type>)
    constexpr basic_table(H&& header, C&& rows = {}) : header{std::forward<H>(header)}, rows_{std::forward<C>(rows)} {}

    constexpr const_iterator begin() const { return rows_.begin(); }
    constexpr iterator begin() { return rows_.begin(); }
    constexpr const_iterator end() const { return rows_.end(); }
    constexpr iterator end() { return rows_.end(); }

    constexpr const_reference at(size_type pos) const { return rows_.at(pos); }
    constexpr reference at(size_type pos) { return rows_.at(pos); }
    constexpr reference operator[](size_type pos) { return rows_[pos]; }
    constexpr const_reference operator[](size_type pos) const { return rows_[pos]; }

    [[nodiscard]] constexpr bool empty() const noexcept { return rows_.empty(); }
    constexpr auto capacity() const { return rows_.capacity(); }
    constexpr auto size() const { return rows_.size(); }
    constexpr void reserve(size_type new_cap) { rows_.reserve(new_cap); }

    template<typename ...Args>
    constexpr reference emplace_back(Args&& ...args) { return rows_.emplace_back(std::forward<Args>(args)...); }

    template <typename T>
    constexpr void push_back(T&& value) { rows_.push_back(std::forward<T>(value)); }

    constexpr auto names() const { return header.names(); }

private:
    container_type rows_;
};


template <typename ...Ts>
struct table : public basic_table<default_header<Ts...>>
{
    using super = basic_table<default_header<Ts...>>;
    using super::super;
};


template <typename RecordT, typename ...Ts>
using record_table = basic_table<record_header<RecordT, Ts...>>;


template <typename ...Ts, typename ...Names>
    requires (std::convertible_to<Names, std::string> && ...) && (sizeof...(Ts) == sizeof...(Names))
constexpr auto make_table(Names&& ...names) -> table<Ts...>
{
    return {{std::forward<Names>(names)...}};
}


template <
    typename... Ts,
    typename HeaderT = typename table<Ts...>::header_type,
    typename ContainerT = typename table<Ts...>::container_type
>
    requires (
        std::convertible_to<HeaderT, typename table<Ts...>::header_type> &&
        std::convertible_to<ContainerT, typename table<Ts...>::container_type>
    )
constexpr auto make_table(HeaderT&& header, ContainerT&& rows = {}) -> table<Ts...>
{
    return {std::forward<HeaderT>(header), std::forward<ContainerT>(rows)};
}


template <template <typename...> typename TableT = table, typename ...Tables>
auto table_join(Tables&& ...tables)
{
    using std::views::zip;

    auto columns = std::tuple_cat(tables.header.columns...);

    auto joined_table = std::apply(
        [](auto ...args)
        {
            return TableT<typename std::decay_t<decltype(args)>::value_type...>{{std::move(args)...}};
        },
        columns);

    joined_table.reserve(std::min({tables.size()...}));

    for (auto const& values : zip(tables...)) {
        auto new_row = std::apply([](auto const& ...args) { return std::tuple_cat(args...); }, values);
        joined_table.push_back(new_row);
    }

    return joined_table;
}

} // namespace tbl

#endif // CPPTABLE_HPP
