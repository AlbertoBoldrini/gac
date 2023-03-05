#ifndef _gac_bit_codec_hpp_
#define _gac_bit_codec_hpp_

namespace gac {
namespace codec {

template<class engine_type> struct symmetrical_bit {

    static constexpr bool is_encoding = engine_type::is_encoding;

    using single_reg = typename engine_type::single_reg;
    using double_reg = typename engine_type::double_reg;
    using probability_type = typename engine_type::guaranteed_reg; 
    
    engine_type &engine;

    symmetrical_bit(engine_type &engine)
        : engine(engine)
    { }

    void process(bool &bit) 
    {
        engine.digest();

        single_reg threshold = (engine.high + engine.low + 1) >> 1;

        // print_registers(engine);

        // std::cout << "| " << std::bitset<engine_type::n_bits>(threshold) << " |" << std::endl;


        if (threshold <= engine.low)
            threshold += single_reg(1) << (engine_type::n_bits - 1);

        if (!engine_type::is_encoding)
            bit = engine.mid >= threshold;

        if (bit)
            engine.low = threshold;
        else
            engine.high = threshold - 1; 
    }
};
 
template<class engine_type> struct asymmetrical_bit {

    static constexpr bool is_encoding = engine_type::is_encoding;

    using single_reg = typename engine_type::single_reg;
    using double_reg = typename engine_type::double_reg;
    using probability_type = typename engine_type::guaranteed_reg; 
    
    engine_type &engine;
    probability_type probability;

    asymmetrical_bit(engine_type &engine, probability_type prob)
        : engine(engine), probability(prob)
    { }

    void process(bool &bit) 
    {
        engine.digest();

        single_reg one_range = (double_reg(engine.high - engine.low) * probability + probability) >> engine_type::n_guaranteed_bits;
        single_reg threshold = engine.low + one_range;

        if (!engine_type::is_encoding)
            bit = engine.mid >= threshold;

        if (bit)
            engine.low = threshold;
        else
            engine.high = threshold - 1; 
    }
};


}}

#endif
