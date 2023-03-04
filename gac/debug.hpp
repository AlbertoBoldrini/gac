#ifndef _gac_debug_hpp_
#define _gac_debug_hpp_

#include <iostream>
#include <iomanip>
#include <bitset>

namespace gac {

    template <class engine_type>
    void print_registers(const engine_type &e) {

        std::cout << "------------------------------------" << std::endl;
        std::cout << "| " << std::bitset<engine_type::n_bits>(high) << " |" << std::endl;

        if (!engine_type::is_encoding) {
            std::cout << "| " << std::bitset<engine_type::n_bits>(mid) << " |" << std::endl;
        }

        std::cout << "| " << std::bitset<engine_type::n_bits>(low)  << " |" << std::endl;
        std::cout << "------------------------------------" << std::endl;
    }
}

#endif