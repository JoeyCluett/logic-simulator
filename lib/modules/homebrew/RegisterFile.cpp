#include <lib/modules/homebrew/RegisterFile.h>
#include <lib/modules/Register.h>
#include <lib/cpp-api/range.h>

#include <stdexcept>

RegisterFile_t::RegisterFile_t(void) 
        : decoder(4, 16) {

    // initialize storage elements
    for(int i : range(0, 16)) {
        this->registers.push_back(Register_t(16));
        this->clock_and_enable.push_back({ AND(), AND() });

        Gate_t dec_output = this->decoder.get_output(i);

        this->clock_and_enable.at(i).first.add_input(dec_output);
        this->clock_and_enable.at(i).second.add_input(dec_output);
    }

    // set data input and output types
    for(int i : range(0, 16)) {
        this->data_input[i].set(FORWARD());
        this->data_output[i].set(WIRE());
    }

    // set data input as input for all registers
    for(int reg_idx : range(0, 16)) {
        Register_t& reg = this->registers.at(reg_idx);
        for(int bit_idx : range(0, 16)) {
            this->data_output[bit_idx].add_input(reg.get_output(bit_idx));
        }
    }
}

std::vector<Gate_t> RegisterFile_t::get_data_output(void) {
    return { this->data_output, this->data_output + 16 };
}

void RegisterFile_t::set_data_input(std::vector<Gate_t> data_inputs) {
    if(data_inputs.size() != 16)
        throw std::runtime_error("RegisterFile_t::set_data_input : data input width must be 16");
}

void RegisterFile_t::set_decode_input(std::vector<Gate_t> decode_inputs) {
    if(decode_inputs.size() != 16)
        throw std::runtime_error("RegisterFile_t::set_decode_input : decode input size must be 4 bits wide");

    this->decoder.set_decoder_input(decode_inputs);
}

void RegisterFile_t::Test(void) {

    {
        logic_init();

        RegisterFile_t rf;

        std::vector<Gate_t> decode_inputs = { 
                    Gate_t(SIGNAL()), Gate_t(SIGNAL()), Gate_t(SIGNAL()), Gate_t(SIGNAL()) };

        std::vector<Gate_t> data_inputs = { 
                    Gate_t(SIGNAL()), Gate_t(SIGNAL()), Gate_t(SIGNAL()), Gate_t(SIGNAL()), 
                    Gate_t(SIGNAL()), Gate_t(SIGNAL()), Gate_t(SIGNAL()), Gate_t(SIGNAL()),
                    Gate_t(SIGNAL()), Gate_t(SIGNAL()), Gate_t(SIGNAL()), Gate_t(SIGNAL()), 
                    Gate_t(SIGNAL()), Gate_t(SIGNAL()), Gate_t(SIGNAL()), Gate_t(SIGNAL()) };

        rf.set_decode_input(decode_inputs);
        rf.set_data_input(data_inputs);

        // TODO: finish

        logic_clean();
    }



}
