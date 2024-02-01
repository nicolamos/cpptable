#pragma once
#ifndef CPPTABLE_HPP
#define CPPTABLE_HPP

#include <concepts>
#include <utility>
#include <vector>
#include <string>
#include <ranges>


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


template <typename T>
auto format_as(const column_info<T>& cinfo) { return cinfo.name; }


template <typename... Ts>
class default_header
{
public:
    using row_type = std::tuple<Ts...>;
    using column_type = std::tuple<column_info<Ts>...>;

    //constexpr static auto size() { return std::tuple_size<row_type>{}; }
    constexpr default_header(column_info<Ts>... cinfo) : columns_{cinfo...} {}

private:
    column_type columns_;
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
    using column_type = typename header_type::column_type;
    using container_type = ContainerT;
    using reference = typename container_type::reference;
    using iterator = typename container_type::iterator;
    using const_iterator = typename container_type::const_iterator;

    template <std::convertible_to<std::string> ...Ts>
    constexpr basic_table(Ts&& ...names) : header_{{std::string(std::forward<Ts>(names))}...} {}

    constexpr const_iterator begin() const { return rows_.begin(); }
    constexpr iterator begin() { return rows_.begin(); }
    constexpr const_iterator end() const { return rows_.end(); }
    constexpr iterator end() { return rows_.end(); }

    template< class... Args >
    constexpr reference emplace_back(Args&&... args) { return rows_.emplace_back(std::forward<Args>(args)...); }

private:
    header_type header_;
    container_type rows_;
};


template <typename... Ts>
struct table : public basic_table <default_header<Ts...>>
{
    using super = basic_table <default_header<Ts...>>;
    using typename super::header_type;
    using typename super::row_type;
    using super::super;
};

} // namespace tbl

#endif // CPPTABLE_HPP
