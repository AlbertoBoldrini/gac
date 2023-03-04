#ifndef _gac_codec_hpp_
#define _gac_codec_hpp_

#include <cstdint>
#include <limits>

#include "stream.hpp"

namespace gac {

template <bool _is_encoding> struct engine {

    static constexpr bool is_encoding = _is_encoding;

    typedef __uint32_t guaranteed_reg;
    typedef __uint64_t single_reg;
    typedef __uint128_t double_reg;

    static constexpr unsigned int n_bytes = sizeof(single_reg);
    static constexpr unsigned int n_bits = sizeof(single_reg) * 8;

    static constexpr unsigned int n_guaranteed_bytes = sizeof(guaranteed_reg);
    static constexpr unsigned int n_guaranteed_bits = sizeof(guaranteed_reg) * 8;

    static constexpr single_reg min_value = std::numeric_limits<single_reg>::min();
    static constexpr single_reg max_value = std::numeric_limits<single_reg>::max();
    static constexpr single_reg guaranteed_range = (single_reg(1) << n_guaranteed_bits) - 1;

    static constexpr single_reg mask_first_byte = single_reg(0xFF) << n_bits - 8;

    static constexpr uint8_t first_byte(const single_reg &reg) {
        return reg >> (n_bits - 8);
    }

    static constexpr single_reg load_byte(const single_reg &reg, const uint8_t &byte) {
        return (reg << 8) | byte;
    }

public:
    stream<is_encoding> io;

    single_reg low;
    single_reg mid;
    single_reg high;

    engine(stream<is_encoding> io)
        : io(io)
        , low(min_value)
        , mid(min_value)
        , high(max_value)
    {   
        if (!is_encoding) {
            for (unsigned int i = 0; i < n_bytes; i++)
                mid = load_byte(mid, io.get()); 
        }
    }

    void finalize() {
        if (is_encoding) {
            for (unsigned int i = 0; i < n_bytes; i++) {
                io.put(first_byte(low));
                low = load_byte(low, 0);
            }
        }
    }

    void digest() {
        while(first_byte(low) == first_byte(high)) {
            if (is_encoding)
                io.put(first_byte(low));
            else 
                mid = load_byte(mid, io.get()); 
            
            low  = load_byte(low, 0);
            high = load_byte(high, 0xFF);
        }

        if (high - low < guaranteed_range) {
            single_reg next_low  = high & mask_first_byte;
            single_reg next_high = low  & mask_first_byte | ~mask_first_byte;
            
            if (next_low - low < high - next_high)
                low = next_low;
            else
                high = next_high;

            digest();
        }
    }
};

}

#endif