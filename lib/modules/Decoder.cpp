#include <lib/modules/Decoder.h>
#include <lib/cpp-api/range.h>

#include <iostream>
#include <vector>
#include <assert.h>

Decoder_t::Decoder_t(size_t address_width) 
        : Decoder_t(address_width, (1 << address_width)) {
}

Decoder_t::Decoder_t(size_t address_width, size_t num_entries) 
        : address_width(address_width), num_entries(num_entries) {

    this->decoder_output_gates.resize(num_entries);
    this->decode_input.resize(address_width);
    this->decode_input_invert.resize(address_width);

    for(auto& g : this->decoder_output_gates)
        g.set(AND());

    for(auto& g : this->decode_input)
        g.set(FORWARD());

    for(auto& g : this->decode_input_invert)
        g.set(NOR());

    for(size_t bit : range(0UL, address_width)) {
        this->decode_input_invert[bit].add_input(this->decode_input[bit]);
    }

    for(size_t addr : range(0UL, num_entries)) {

        Gate_t& and_gate = this->decoder_output_gates.at(addr);

        for(int j : range(0UL, address_width)) {
            int bit_value = (addr >> j) & 0x01;

            if(bit_value) and_gate.add_input(this->decode_input.at(j));
            else          and_gate.add_input(this->decode_input_invert.at(j));
        }
    }
}

void Decoder_t::set_decoder_input(std::vector<Gate_t> decode_input) {
    if(decode_input.size() != this->address_width)
        throw std::runtime_error("Decoder_t::set_decoder_input : decoder input width must match address width");

    for(size_t idx : range(0UL, this->address_width))
        this->decode_input.at(idx).add_input(decode_input.at(idx));
}

Gate_t Decoder_t::get_output(size_t address) {
    if(address >= this->num_entries)
        throw std::runtime_error("Decoder_t::get_output : requested address is out of range");

    return this->decoder_output_gates.at(address);
}

void Decoder_t::Test(void) {
    {
        logic_init();

        Decoder_t decoder(4);

        std::vector<Gate_t> decode_inputs = { 
                    Gate_t(SIGNAL()), 
                    Gate_t(SIGNAL()), 
                    Gate_t(SIGNAL()), 
                    Gate_t(SIGNAL()) };

        decoder.set_decoder_input(decode_inputs);

        for(int i : range(0, 16)) {
            for(int b : range(0, 4)) {
                int bit_value = (i >> b) & 0x01;
                decode_inputs[b].set_output(bit_value);
            }

            logic_gate_simulate();

            for(int j : range(0, 16)) {                
                if(j == i) assert(decoder.get_output(j).get_output() == OUTPUT_ONE);
                else       assert(decoder.get_output(j).get_output() == OUTPUT_ZERO);
            }
        }

        logic_allocator_debug_info();

        for(auto& g : decode_inputs) {
            free(g.internal_gate);
        }

        logic_clean();
    }

    {
        logic_init();
    
        Decoder_t decoder(8, 173);

        std::vector<Gate_t> decode_inputs = { 
                    Gate_t(SIGNAL()), 
                    Gate_t(SIGNAL()), 
                    Gate_t(SIGNAL()), 
                    Gate_t(SIGNAL()), 
                    Gate_t(SIGNAL()), 
                    Gate_t(SIGNAL()), 
                    Gate_t(SIGNAL()), 
                    Gate_t(SIGNAL()) };

        decoder.set_decoder_input(decode_inputs);

        for(int i : range(0, 173)) {

            for(int b : range(0, 8)) {
                int bit_value = (i >> b) & 0x01;
                decode_inputs[b].set_output(bit_value);
            }

            logic_gate_simulate();

            for(int j : range(0, 173)) {
                if(j == i) assert(decoder.get_output(j).get_output() == OUTPUT_ONE);
                else       assert(decoder.get_output(j).get_output() == OUTPUT_ZERO);
            }
        }

        logic_allocator_debug_info();

        for(auto& g : decode_inputs) {
            free(g.internal_gate);
        }

        logic_clean();
    }

}

