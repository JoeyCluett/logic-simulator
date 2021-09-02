#pragma once

#include <vector>
#include <utility>

#include <lib/logic.h>
#include <lib/cpp-api/gate-wrapper.h>

#include <lib/modules/Register.h>  // data storage elements
#include <lib/modules/Decoder.h>   // for decode logic

struct RegisterFile_t {

    std::vector<Register_t> registers;
    std::vector<std::pair<Gate_t, Gate_t>> clock_and_enable; // pair.first=data, pair.second=enable

    Decoder_t decoder;

    Gate_t data_input[16];  // forward type
    Gate_t data_output[16]; // wire type

    //
    // register file is always 16x 16-bit registers
    //
    RegisterFile_t(void);

    //
    // set inputs for decoder
    //
    void set_decode_input(std::vector<Gate_t> decode_inputs);

    //
    // set inputs for data bus
    //
    void set_data_input(std::vector<Gate_t> data_inputs);

    //
    // set clock input
    // this is a single bit
    //
    void set_clock_input(Gate_t gate);

    //
    // get vector of all output signals
    //
    std::vector<Gate_t> get_data_output(void);

    //
    // test RegisterFile_t
    //
    static void Test(void);
};

