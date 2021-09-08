#include <lib/modules/DecoderWEnable.h>

DecoderWEnable_t::DecoderWEnable_t(size_t address_width) 
    :
        internal_decoder(address_width) 
{

}

DecoderWEnable_t::DecoderWEnable_t(size_t address_width, size_t num_entries)
    :
        internal_decoder(address_width, num_entries) 
{
    
}

void DecoderWEnable_t::set_decoder_input(std::vector<Gate_t> decode_input) {

}

auto DecoderWEnable_t::get_outputs(void) -> std::vector<Gate_t> {

}

