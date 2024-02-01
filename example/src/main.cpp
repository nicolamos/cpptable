#include <iostream>
#include <tuple>
#include <fmt/core.h>
#include <fmt/ranges.h>
#include <fmt/ostream.h>
#include <mp-units/format.h>
#include <mp-units/ostream.h>
#include <mp-units/systems/si/si.h>

#include "cpptable/cpptable.hpp"

using namespace mp_units;


namespace tbl
{

template <Reference auto r, typename T>
struct column_info<quantity<r, T>>
{
    using value_type = quantity<r, T>;
    constexpr operator std::string() const { return fmt::format("({}, {})", name, std::string(unit_symbol(get_unit(r)))); }
    std::string name;
};

}


template <Reference auto r, typename T>
struct fmt::formatter<tbl::column_info<quantity<r, T>>> : fmt::formatter<std::pair<string_view, decltype(get_unit(r))>>
{
    using reference_type = decltype(get_unit(r));
    using Q = quantity<r, T>;
    using super = fmt::formatter<std::pair<string_view, reference_type>>;
    using column_type = tbl::column_info<Q>;
    auto format(const column_type& cinfo, format_context& ctx) const
    {
        return super::format(std::make_pair(cinfo.name, get_unit(r)), ctx);
    }
};


int main()
{
    using si::unit_symbols::cm;
    using si::unit_symbols::s;

    std::cout << fmt::format("{}\n", 34 * cm);

    using table = tbl::table<int, quantity<isq::speed[cm / s], float>>;

    table t = {"c1", "c2"};

    t.emplace_back(2, 3 * cm / s);

    for (auto const& row : t) {
        fmt::println("{}", row);
    }

    fmt::println("{}", t.header);

    return 0;
}
