#pragma once
#ifndef CPPTABLE_HPP
#define CPPTABLE_HPP

#include <vector>

namespace tbl
{

template <typename T>
struct position
{
    T x;
    T y;
    T z;
};

template <typename RowT, typename ContainerT = std::vector<RowT>>
class basic_table
{
public:
    using row_type = RowT;
    using container_type = ContainerT;

    basic_table() = default;
    basic_table(std::initializer_list<row_type> args) : rows_{args} {}
    basic_table(container_type rows) : rows_{std::move(rows)} {}

    auto begin() const { return rows_.begin(); }
    auto begin() { return rows_.begin(); }
    auto end() const { return rows_.end(); }
    auto end() { return rows_.end(); }

private:
    container_type rows_;
};

} // namespace tbl

#endif // CPPTABLE_HPP
