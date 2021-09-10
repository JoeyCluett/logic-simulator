#pragma once

#include <lib/logic.h>
#include <lib/cpp-api/gate-wrapper.h>
#include <lib/modules/Decoder.h>

#include <vector>

struct DemuxGeneric_t {

    std::vector<std::vector<Gate_t>> demux_v_v; // vector of vector of tristate buffers
    std::vector<Gate_t> input_bus; // FORWARD
    Decoder_t decoder;

    DemuxGeneric_t(size_t addr_width, size_t n_entries, size_t bit_width);

    void set_input(std::vector<Gate_t> in);

    auto get_output(void) -> std::vector<std::vector<Gate_t>>;
    auto get_output(size_t idx) -> std::vector<Gate_t>;

    void set_select_input(std::vector<Gate_t> sel);

};
