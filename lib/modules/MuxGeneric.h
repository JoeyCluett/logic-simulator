#pragma once

#include <lib/logic.h>
#include <lib/cpp-api/gate-wrapper.h>

#include <lib/modules/Decoder.h>

#include <vector>

struct MuxGeneric_t {

    std::vector<std::vector<Gate_t>> mux_v_v; // vector of vector of tristate buffers

    std::vector<Gate_t> output_bus;

    Decoder_t decoder;

    MuxGeneric_t(size_t select_width, size_t n_entries, size_t bit_width);

    void set_input(size_t entry_n, std::vector<Gate_t> bits);

    auto get_output(void) -> std::vector<Gate_t>;

    void set_select_input(std::vector<Gate_t> sel);
};

