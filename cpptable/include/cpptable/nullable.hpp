#pragma once
#ifndef CPPTABLE_NULLABLE_HPP
#define CPPTABLE_NULLABLE_HPP

#include "cpptable.hpp"
#include "utils.hpp"

namespace tbl
{

template <typename ...Ts>
using nullable_table = table<formattable_optional<Ts>...>;

} // namespace tbl

#endif // CPPTABLE_NULLABLE_HPP
