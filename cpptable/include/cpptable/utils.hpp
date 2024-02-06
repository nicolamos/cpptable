#pragma once
#ifndef CPPTABLE_UTILS_HPP
#define CPPTABLE_UTILS_HPP

#include <optional>

namespace tbl
{

template <typename T>
struct formattable_optional : public std::optional<T>
{
    using super = std::optional<T>;
    using value_type = typename super::value_type;
    using super::super;
};

} // namespace tbl

#endif // CPPTABLE_UTILS_HPP
