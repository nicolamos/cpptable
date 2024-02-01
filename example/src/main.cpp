#include <iostream>
#include <tuple>
#include <fmt/core.h>
#include <fmt/ranges.h>
#include <fmt/ostream.h>
#include <mp-units/format.h>
#include <mp-units/systems/si/si.h>

#include "cpptable/cpptable.hpp"

using namespace mp_units;


int main()
{
    using si::unit_symbols::cm;
    using si::unit_symbols::s;

    std::cout << fmt::format("{}", 34 * cm) << std::endl;

    using table = tbl::table<int, quantity<isq::speed[cm / s], float>>;

    table t = {"c1", "c2"};

    t.emplace_back(2, 3 * cm / s);

    for (auto const& row : t) {
        std::apply([](auto&&... args) { (fmt::print("{}, ", args),...); }, row);
        fmt::println("");
    }

    return 0;
}
