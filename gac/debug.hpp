#ifndef _gac_debug_hpp_
#define _gac_debug_hpp_

#include <iostream>
#include <iomanip>
#include <bitset>

namespace gac {

    template <class engine_type>
    void print_registers(const engine_type &e) {

        std::cout << "------------------------------------" << std::endl;
        std::cout << "| " << std::bitset<engine_type::n_bits>(e.high) << " |" << std::endl;

        if (!engine_type::is_encoding) {
            std::cout << "| " << std::bitset<engine_type::n_bits>(e.mid) << " |" << std::endl;
        }

        std::cout << "| " << std::bitset<engine_type::n_bits>(e.low)  << " |" << std::endl;
        std::cout << "------------------------------------" << std::endl;
    }
}

#endif