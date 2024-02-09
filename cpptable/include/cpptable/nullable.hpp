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


template <typename ...Ts, typename ...Names>
    requires (std::convertible_to<Names, std::string> && ...) && (sizeof...(Ts) == sizeof...(Names))
constexpr auto make_table(nullable_t, Names&& ...names) -> nullable_table<Ts...>
{
    return {{std::forward<Names>(names)...}};
}


template <
    typename... Ts,
    typename HeaderT = typename nullable_table<Ts...>::header_type,
    typename ContainerT = typename nullable_table<Ts...>::container_type>
    requires (
        std::convertible_to<HeaderT, typename nullable_table<Ts...>::header_type> &&
        std::convertible_to<ContainerT, typename nullable_table<Ts...>::container_type>)
constexpr auto make_table(nullable_t, HeaderT&& header, ContainerT&& rows = {}) -> nullable_table<Ts...>
{
    return {std::forward<HeaderT>(header), std::forward<ContainerT>(rows)};
}

} // namespace tbl

#endif // CPPTABLE_NULLABLE_HPP
