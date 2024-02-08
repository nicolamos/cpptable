#pragma once
#ifndef CPPTABLE_NULLABLE_HPP
#define CPPTABLE_NULLABLE_HPP

#include <type_traits>
#include <concepts>
#include <string>
#include <utility>

#include "cpptable.hpp"
#include "utils.hpp"

namespace tbl
{

template <typename ...Ts>
using nullable_table = table<formattable_optional<Ts>...>;


struct nullable_t {};

inline constexpr nullable_t nullable{};


// template <typename ...Ts, std::convertible_to<std::string> ...Names>
// constexpr auto make_table(nullable_t, Names&& ...names) -> std::enable_if_t<sizeof...(Ts) == sizeof...(Names), nullable_table<Ts...>>
// {
//     return { names... };
// }


// template <typename ...Ts>
// constexpr auto make_table(nullable_t, column_info<Ts> ...cinfo) -> nullable_table<Ts...>
// {
//     return { std::move(cinfo)... };
// }


template <typename ...Ts, typename ContainerT = typename nullable_table<Ts...>::container_type>
constexpr auto make_table(nullable_t, column_info<Ts> ...cinfo, ContainerT&& rows = {}) -> nullable_table<Ts...>
{
    return { std::move(cinfo)..., std::forward<ContainerT>(rows) };
}

} // namespace tbl

#endif // CPPTABLE_NULLABLE_HPP
