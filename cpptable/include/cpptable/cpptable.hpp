#pragma once
#ifndef CPPTABLE_HPP
#define CPPTABLE_HPP

#include <vector>
#include <string>

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

    constexpr operator std::string() const { return name; }

    std::string name;
};


class default_header
{
public:

private:

};


template <
    typename HeaderT = default_header,
    typename ContainerT = std::vector<typename HeaderT::row_type>
>
class basic_table : private HeaderT
{
public:
    using super = HeaderT;
    using typename super::header_type;
    using typename super::row_type;
    using container_type = ContainerT;
    using iterator = typename container_type::iterator;
    using const_iterator = typename container_type::const_iterator;

    basic_table() = default;
    basic_table(std::initializer_list<row_type> args) : rows_{args} {}
    basic_table(container_type rows) : rows_{std::move(rows)} {}

    constexpr const_iterator begin() const { return rows_.begin(); }
    constexpr iterator begin() { return rows_.begin(); }
    constexpr const_iterator end() const { return rows_.end(); }
    constexpr iterator end() { return rows_.end(); }

private:
    container_type rows_;
};


// template <typename... ColumnT>
// class table : public basic_table <>
// {
// public:

// private:
// };

} // namespace tbl

#endif // CPPTABLE_HPP
