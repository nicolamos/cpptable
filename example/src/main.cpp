#include <iostream>
#include <tuple>
#include <algorithm>
#include <fmt/core.h>
#include <fmt/ranges.h>
#include <fmt/ostream.h>
#include <mp-units/format.h>
#include <mp-units/ostream.h>
#include <mp-units/systems/si/si.h>

#include "cpptable/cpptable.hpp"

using namespace mp_units;


template <Reference auto r, typename T>
struct fmt::formatter<tbl::column_info<quantity<r, T>>> : fmt::formatter<std::pair<string_view, decltype(get_unit(r))>>
{
    using unit_type = decltype(get_unit(r));
    using Q = quantity<r, T>;
    using super = fmt::formatter<std::pair<string_view, unit_type>>;
    using column_type = tbl::column_info<Q>;
    auto format(const column_type& cinfo, format_context& ctx) const
    {
        return super::format(std::make_pair(cinfo.name, get_unit(r)), ctx);
    }
};


namespace tbl
{

template <Reference auto... Rs>
struct qtable : public table<quantity<Rs>...>
{
    using super = table<quantity<Rs>...>;
    using super::super;
};

}


int main()
{
    namespace ranges = std::ranges;
    using si::unit_symbols::cm;
    using si::unit_symbols::s;

    std::cout << fmt::format("{}\n", 34 * cm);

    using table = tbl::table<int, quantity<isq::speed[cm / s], float>>;

    table t = {"c1", "c2"};

    t.emplace_back(2, 3 * cm / s);
    t.emplace_back(4, 4.0e-1 * cm / s);

    ranges::for_each(
        t,
        [](const auto& c) { fmt::println("{}", c); },
        [](const auto& row) { return std::get<1>(row); }
    );

    for (auto const& row : t) {
        fmt::println("{}", row);
    }

    fmt::println("{}", t.header);

    using qtable = tbl::qtable<isq::length[cm], isq::time[si::second]>;
    qtable qt{"c1", "c2"};

    fmt::println("{}", qt.header);

    return 0;
}
