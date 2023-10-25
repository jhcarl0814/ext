#include "ext_bitset.hpp"

#include <algorithm>

void test_bitset()
{
    std::bitset bs = "0011"_bsr;
    std::rotate(std::begin(bs), std::begin(bs) + 1, std::end(bs));
    assert(bs == "0110"_bsr);
    std::shift_left(std::begin(bs), std::end(bs), 1);
    assert(bs == "1100"_bsr);
}
