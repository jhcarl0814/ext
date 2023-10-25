
#include "ext_hilbert.hpp"

#include <ranges>
#include "ext_debug.hpp"

void test_hilbert()
{
    for(std::size_t a1 : std::views::iota(0b00, 0b100))
    {
        for(std::size_t a2 : std::views::iota(0b00, 0b100))
        {
            ext_debug_log((std::bitset<2>(a1),
                              std::bitset<2>(a2),
                              space_coordinate_to_hilbert_coordinate_pieces(std::to_array({
                                  std::bitset<2>(a1),
                                  std::bitset<2>(a2),
                              })),
                              hilbert_coordinate_pieces_to_hilbert_coordinate(space_coordinate_to_hilbert_coordinate_pieces(std::to_array({
                                  std::bitset<2>(a1),
                                  std::bitset<2>(a2),
                              })))),
                qDebug_compact());
        }
    }

    for(std::size_t a1 : std::views::iota(0b00, 0b100))
    {
        for(std::size_t a2 : std::views::iota(0b00, 0b100))
        {
            for(std::size_t a3 : std::views::iota(0b00, 0b100))
            {
                ext_debug_log((std::bitset<2>(a1),
                                  std::bitset<2>(a2),
                                  std::bitset<2>(a3),
                                  space_coordinate_to_hilbert_coordinate_pieces(std::to_array({
                                      std::bitset<2>(a1),
                                      std::bitset<2>(a2),
                                      std::bitset<2>(a3),
                                  })),
                                  hilbert_coordinate_pieces_to_hilbert_coordinate(space_coordinate_to_hilbert_coordinate_pieces(std::to_array({
                                      std::bitset<2>(a1),
                                      std::bitset<2>(a2),
                                      std::bitset<2>(a3),
                                  })))),
                    qDebug_compact());
            }
        }
    }
    for(std::size_t h : std::views::iota(0b00, 0b10000))
    {
        ext_debug_log((std::bitset<4>(h), (hilbert_coordinate_pieces_to_space_coordinate(hilbert_coordinate_to_hilbert_coordinate_pieces<2, 2>(std::bitset<4>(h))))), qDebug_compact());
    }
    for(std::size_t h : std::views::iota(0b00, 0b1000000))
    {
        ext_debug_log((std::bitset<6>(h),
                          (hilbert_coordinate_to_hilbert_coordinate_pieces<2, 3>(std::bitset<6>(h))),
                          (hilbert_coordinate_pieces_to_space_coordinate(hilbert_coordinate_to_hilbert_coordinate_pieces<2, 3>(std::bitset<6>(h))))),
            qDebug_compact());
    }
}
