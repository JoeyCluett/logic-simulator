#pragma once

#include <vector>

#include <lib/logic.h>
#include <lib/cpp-api/gate-wrapper.h>

#include <lib/modules/Decoder.h>

struct DecoderWEnable_t {

    Decoder_t internal_decoder;

    std::vector<Gate_t> output_enable_ands;

    //std::vector<Gate_t> decoder_output_gates;
    //std::vector<Gate_t> decode_input_invert;
    //std::vector<Gate_t> decode_input;
    size_t address_width;
    size_t num_entries;

    //
    // ctor must specify address width
    //
    DecoderWEnable_t(size_t address_width);

    //
    // ctor, specify address width AND how many 
    // entries are needed in that address width
    //
    DecoderWEnable_t(size_t address_width, size_t num_entries);

    //
    // set the decoder inputs
    //
    void set_decoder_input(std::vector<Gate_t> decode_input);

    auto get_outputs(void) -> std::vector<Gate_t>;
};
