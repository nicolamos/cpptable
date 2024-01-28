#include <iostream>
#include <tuple>
#include <fmt/core.h>
#include <fmt/ranges.h>

#include "cpptable/cpptable.hpp"


int main()
{
    std::cout << "CIAO" << std::endl;

    using row_type = std::tuple<int, int>;
    using table = tbl::basic_table<row_type, tbl::meta_data<>>;

    table t = {{2, 3}};

    for (auto& row : t) {
        fmt::println("{}", row);
    }

    return 0;
}
