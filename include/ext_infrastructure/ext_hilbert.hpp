#ifndef EXT_HILBERT_H
#define EXT_HILBERT_H

#include <array>
#include <bitset>

//https://aip.scitation.org/doi/abs/10.1063/1.1751381 Programming the Hilbert curve: AIP Conference Proceedings: Vol 707, No 1
//https://stackoverflow.com/a/10384110/8343353
//https://stackoverflow.com/a/68430271/8343353

template<std::size_t num_bits, std::size_t num_dims>
constexpr std::array<std::bitset<num_bits>, num_dims> hilbert_coordinate_pieces_to_space_coordinate(std::array<std::bitset<num_bits>, num_dims> X)
{
    {
        // Gray decode by H ^ (H/2)
        std::bitset<num_bits> t = X[num_dims - 1] >> 1;
        for(int i = num_dims - 1; i > 0; i--) // https://stackoverflow.com/a/10384110
            X[i] ^= X[i - 1];
        X[0] ^= t;
    }

    // Undo excess work
    for(std::bitset<num_bits> Q = 2, P = 1; Q.any(); Q <<= 1, P <<= 1, P |= 1)
    {
        for(std::size_t i = num_dims - 1; i != static_cast<std::size_t>(0) - 1; --i)
        {
            if((X[i] & Q).any())
            { // invert low bits of X[0]
                X[0] ^= P;
            }
            else
            { // exchange low bits of X[i] and X[0]
                std::bitset<num_bits> t = (X[0] ^ X[i]) & P;
                X[0] ^= t;
                X[i] ^= t;
            }
        }
    }

    return X;
}

template<std::size_t num_bits, std::size_t num_dims>
constexpr std::array<std::bitset<num_bits>, num_dims> space_coordinate_to_hilbert_coordinate_pieces(std::array<std::bitset<num_bits>, num_dims> X)
{
    // Inverse undo
    for(std::bitset<num_bits> Q = std::bitset<num_bits>(1) << (num_bits - 1), P = std::bitset<num_bits>(0).set() >> 1; Q.any(); Q >>= 1, P >>= 1)
    {
        for(std::size_t i = 0; i != num_dims; ++i)
        {
            if((X[i] & Q).any())
                X[0] ^= P;
            else
            {
                std::bitset<num_bits> t = (X[0] ^ X[i]) & P;
                X[0] ^= t;
                X[i] ^= t;
            }
        }
    } // exchange

    // Gray encode
    for(std::size_t i = 1; i != num_dims; ++i)
        X[i] ^= X[i - 1];

    std::bitset<num_bits> t = 0;
    for(std::bitset<num_bits> Q = std::bitset<num_bits>(1) << (num_bits - 1), P = std::bitset<num_bits>(0).set() >> 1; Q.any(); Q >>= 1, P >>= 1)
    {
        if((X[num_dims - 1] & Q).any())
            t ^= P;
    }

    for(std::size_t i = 0; i != num_dims; ++i)
        X[i] ^= t;

    return X;
}

template<std::size_t num_bits, std::size_t num_dims>
constexpr std::array<std::bitset<num_bits>, num_dims> hilbert_coordinate_to_hilbert_coordinate_pieces(std::bitset<num_bits * num_dims> H)
{
    std::array<std::bitset<num_bits>, num_dims> X;
    for(std::size_t dim = 0; dim != num_dims; ++dim)
    {
        for(std::size_t bit = num_bits - 1; bit != static_cast<std::size_t>(0) - 1; --bit)
        {
            X[dim][bit] = H[((num_bits * num_dims - 1) - dim) - ((num_bits - 1) - bit) * num_dims];
        }
    }
    return X;
}

template<std::size_t num_bits, std::size_t num_dims>
constexpr std::bitset<num_bits * num_dims> hilbert_coordinate_pieces_to_hilbert_coordinate(std::array<std::bitset<num_bits>, num_dims> Htrans)
{
    std::bitset<num_bits * num_dims> H;
    for(std::size_t i = num_bits * num_dims - 1; i != static_cast<std::size_t>(0) - 1; i--) {
        H[i] = Htrans[((num_bits * num_dims - 1) - i) % num_dims][i / num_dims];
    }
    return H;
}

void test_hilbert();

#endif // EXT_HILBERT_H
