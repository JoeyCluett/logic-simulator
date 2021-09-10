#include <lib/modules/DemuxGeneric.h>

#include <lib/cpp-api/range.h>

DemuxGeneric_t::DemuxGeneric_t(size_t addr_width, size_t n_entries, size_t bit_width) 
    :
        input_bus(VectorInit_(bit_width, FORWARD)),
        decoder(addr_width, n_entries)
{

    for(size_t idx : range(0ul, n_entries)) {
        this->demux_v_v.push_back(VectorInit_(bit_width, TRISTATE_BUFFER));
    
        std::vector<Gate_t>& output_buffers = this->demux_v_v.back();
        VectorConnect_(output_buffers, this->input_bus);

        Gate_t decoder_output = this->decoder.get_output(idx);
        for(Gate_t tristatebuffer : output_buffers) {
            tristatebuffer.tristate_set_enable(decoder_output);
        }
    }
}

void DemuxGeneric_t::set_input(std::vector<Gate_t> in) {
    VectorConnect_(this->input_bus, in);
}

auto DemuxGeneric_t::get_output(void) -> std::vector<std::vector<Gate_t>> {
    return this->demux_v_v;
}

auto DemuxGeneric_t::get_output(size_t idx) -> std::vector<Gate_t> {
    return this->demux_v_v.at(idx);
}

void DemuxGeneric_t::set_select_input(std::vector<Gate_t> sel) {
    this->decoder.set_decoder_input(sel);
}
