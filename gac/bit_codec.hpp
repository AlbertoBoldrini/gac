#ifndef _gac_bit_codec_hpp_
#define _gac_bit_codec_hpp_

namespace gac {
 
template<class engine_type> struct bit_codec {

    static constexpr bool is_encoding = engine_type::is_encoding;

    using single_reg = typename engine_type::single_reg;
    using double_reg = typename engine_type::double_reg;
    using probability_type = typename engine_type::guaranteed_reg; 
    
    engine_type &engine;
    probability_type probability;

    bit_codec(engine_type &engine, probability_type prob)
        : engine(engine), probability(prob)
    { }

    void encode(bool &bit) 
    {
        engine.digest();

        double_reg current_range = double_reg(engine.high - engine.low) + 1;
        single_reg threshold = engine.low + ((current_range * probability) >> 16);

        if (!engine_type::is_encoding)
            bit = engine.mid >= threshold;

        if (bit)
            engine.low = threshold;
        else
            engine.high = threshold - 1; 
    }
};

}

#endif
