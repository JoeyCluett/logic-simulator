#include <lib/modules/homebrew/AssertConstants.h>

#include <lib/cpp-api/range.h>

AssertConstants_t::AssertConstants_t(void) {

    for(int i : range(0, 16)) {
        this->buffer_minus_one[i].set(TRISTATE_BUFFER());
        this->buffer_zero[i].set(TRISTATE_BUFFER());
        this->buffer_one[i].set(TRISTATE_BUFFER());

        this->buffer_zero[i].tristate_set_data(ZERO);
        this->buffer_minus_one[i].tristate_set_data(ONE);
    }

    this->buffer_one[0].tristate_set_data(ONE);
    for(int i : range(1, 16))
        this->buffer_one[i].tristate_set_data(ZERO);
}

void AssertConstants_t::attach_to_output_bus(std::vector<Gate_t> output_bus) {
    if(output_bus.size() != 16)
        throw std::runtime_error("AssertConstants_t::attach_to_output_bus : output bus width must be 16");

    for(int i : range(0, 16)) {
        output_bus.at(i).add_input(this->buffer_one[i]);
        output_bus.at(i).add_input(this->buffer_zero[i]);
        output_bus.at(i).add_input(this->buffer_minus_one[i]);
    }
}

void AssertConstants_t::set_enable_one(Gate_t gate) {
    for(int i : range(0, 16))
        this->buffer_one[i].tristate_set_enable(gate);
}

void AssertConstants_t::set_enable_minus_one(Gate_t gate) {
    for(int i : range(0, 16))
        this->buffer_minus_one[i].tristate_set_enable(gate);
}

void AssertConstants_t::set_enable_zero(Gate_t gate) {
    for(int i : range(0, 16))
        this->buffer_zero[i].tristate_set_enable(gate);
}


