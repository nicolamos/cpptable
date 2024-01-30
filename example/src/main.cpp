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

    std::cout << fmt::format("{}", 34 * cm) << std::endl;

    using table = tbl::basic_table<>;

    table t = {{2, 3}};

    for (auto& row : t) {
        fmt::println("{}", row);
    }

    return 0;
}
