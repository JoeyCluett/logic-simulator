#pragma once

#include <vector>

#include <lib/logic.h>
#include <lib/cpp-api/gate-wrapper.h>
#include <lib/modules/WideGates.h>

//
// DO NOT USE THIS FOR LARGE ADDRESS SPACES!!!
// a few dozen entries is fine, ~4 billion is NOT!!!!
//
struct Decoder_t {

    std::vector<Gate_t> decoder_output_gates;
    std::vector<Gate_t> decode_input_invert;
    std::vector<Gate_t> decode_input;
    size_t address_width;
    size_t num_entries;

    //
    // ctor must specify address width
    //
    Decoder_t(size_t address_width);

    //
    // ctor, specify address width AND how many 
    // entries are needed in that address width
    //
    Decoder_t(size_t address_width, size_t num_entries);

    //
    // set the decoder inputs
    //
    void set_decoder_input(std::vector<Gate_t> decode_input);

    //
    // get output signal from specific decoder output
    //
    Gate_t get_output(size_t addr_idx);

    //
    // test Decoder_t
    //
    static void Test(void);

};
