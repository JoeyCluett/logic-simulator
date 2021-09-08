#include <lib/modules/MuxGeneric.h>

#include <lib/cpp-api/range.h>

MuxGeneric_t::MuxGeneric_t(size_t select_width, size_t n_entries, size_t bit_width) 
    :
        decoder(select_width, n_entries),
        output_bus(VectorInit_(bit_width, WIRE)) {

    std::vector<Gate_t> decode_outputs = this->decoder.decoder_output_gates;

    for(size_t i = 0ul; i < n_entries; i++) {
        this->mux_v_v.push_back(VectorInit_(bit_width, TRISTATE_BUFFER));
    }

    for(size_t idx : range(0ul, n_entries)) {
        Gate_t decoder_output_line = decode_outputs.at(idx);

        for(auto& g : this->mux_v_v.at(idx))
            g.tristate_set_enable(decoder_output_line);

        VectorConnect_(this->output_bus, this->mux_v_v.at(idx));
    }
}

void MuxGeneric_t::set_input(size_t entry_n, std::vector<Gate_t> bits) {
    VectorConnect_(this->mux_v_v.at(entry_n), bits);
}

auto MuxGeneric_t::get_output(void) -> std::vector<Gate_t> {
    return this->output_bus;
}

void MuxGeneric_t::set_select_input(std::vector<Gate_t> sel) {
    this->decoder.set_decoder_input(sel);
}
