#ifndef _gac_stream_hpp_
#define _gac_stream_hpp_

#include <cstdint>
#include <istream>
#include <ostream>

namespace gac {

    template <bool is_encoding> 
        struct stream;

    template <> 
        struct stream<true> {
            std::ostream &s;

            stream(std::ostream &s): s(s) {}

            std::uint8_t get() { 
                return 0; 
            }

            void put(std::uint8_t byte) {
                s.put(char(byte));
            }
        };

    template <> 
        struct stream<false> {
            std::istream &s;

            stream(std::istream &s): s(s) {}

            std::uint8_t get() { 
                char c;
                s.get(c);
                return std::uint8_t(c); 
            }

            void put(std::uint8_t byte) {
                
            }
        };
}

#endif