#include <iostream>
#include <tuple>
#include <algorithm>
#include <ranges>
#include <optional>
#include <fmt/core.h>
#include <fmt/ranges.h>
#include <fmt/ostream.h>
#include <fmt/format.h>
#include <mp-units/format.h>
#include <mp-units/ostream.h>
#include <mp-units/systems/si/si.h>

#include "cpptable/cpptable.hpp"
#include "cpptable/format.hpp"

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


template <typename T>
struct position
{
    T x{}, y{}, z{};
};


template <typename T>
constexpr auto format_as(const position<T>& pos) { return std::make_tuple(pos.x, pos.y, pos.z); }


int main()
{
    namespace ranges = std::ranges;
    namespace views = std::views;
    using si::unit_symbols::cm;
    using si::unit_symbols::s;
    using qpos = position<quantity<isq::length[si::metre]>>;

    std::cout << fmt::format("{}\n", 34 * cm);

    using table = tbl::table<qpos, quantity<isq::speed[cm / s], float>>;

    table t{
        {{2 * cm, 1*cm, -2*cm}, 3 * cm / s},
        {{4 * cm, 100*cm, 0.3*cm}, 4.0e-1 * cm / s}
    };

    t.header = {"c1", "c2"};

    t.emplace_back(qpos{5 * cm, 2*cm, 20*cm}, 3 * si::metre / s);

    ranges::for_each(
        t,
        [](const auto& c) { fmt::println("{}", c); },
        [](const auto& row) { return std::get<1>(row); }
    );

    for (auto const& row : t) {
        fmt::println("{}", row);
    }

    for (auto const& c : t | views::elements<1>) {
        fmt::println("{}", c);
    }

    fmt::println("{}", t.header);

    using qtable = tbl::qtable<isq::length[cm], isq::time[si::second]>;
    qtable qt = { {2*cm, 3 * si::femto<s>} };

    qt.header = {"c1", "c2"};

    qt.emplace_back(0.5*cm, 3.2 * s);

    fmt::println("{}", qt.header);

    auto column = views::elements<1>(qt);
    // auto vec_col = views::elements<1>(qt) | ranges::to<std::vector>(); // C++23
    // std::vector vec_col_2(std::from_range, views::elements<0>(qt)); // C++17 class template argument deduction

    fmt::println("column 1: {}", column);
    // fmt::println("vector column 1: {}", vec_col);
    // fmt::println("vector column 0: {}", vec_col_2);

    using speed_t = quantity<isq::speed[cm/s], double>;
    using vec_col_t = std::vector<speed_t>;
    using ragged_table = tbl::table<std::string, vec_col_t>;
    ragged_table rt{
        {"pippo", {2*cm/s, 4.5*cm/s}},
        {"pluto", { 4.1*si::metre / s}}
    };

    rt.header = {"name", "speed"};

    fmt::println("speed: {}", views::elements<1>(rt));

    // Simple join operation of table A and table B (number of rows is the minimum between the size of two tables)
    auto tA = tbl::make_table<std::string, int>({"C0", "C1"}, {{"ciao", 1}, {"ciao1", 2}});
    auto tB = tbl::make_table<float, float>("B0", "B1");

    tB.emplace_back(0.1, 0.3);
    tB.emplace_back(0.5, 0.7);

    auto tjoin = tbl::table_join(tA, tB);

    fmt::println("{}", tjoin);

    // Missing values
    // using nullable_table = tbl::nullable_table<std::string, int>;
    auto nt = tbl::make_table<std::string, int>(tbl::nullable, { "C0", "C1" }, { {"CIAO", std::nullopt} });

    fmt::println("{}", nt);

    return 0;
}
