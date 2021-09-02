#include <lib/modules/Multiplexer.h>
#include <lib/cpp-api/range.h>

#include <vector>
#include <stdexcept>

Multiplexer_t::Multiplexer_t(size_t width) 
        : not_gate(NOR()), select_input(FORWARD()) {

    this->one_gates.resize(width);
    this->zero_gates.resize(width);
    this->or_gates.resize(width);

    this->not_gate.add_input(this->select_input);

    for(size_t i : range(0UL, width)) {
        this->one_gates.at(i).set(AND());
        this->zero_gates.at(i).set(AND());
        this->or_gates.at(i).set(OR());

        this->zero_gates.at(i).add_input(this->not_gate);
        this->one_gates.at(i).add_input(this->select_input);
    
        this->or_gates.at(i).add_input(this->zero_gates.at(i));
        this->or_gates.at(i).add_input(this->one_gates.at(i));
    }
}

void Multiplexer_t::set_select_input(Gate_t gate) {
    this->select_input.add_input(gate);
}

std::vector<Gate_t> Multiplexer_t::get_output(void) {
    return this->or_gates;
}

void Multiplexer_t::set_zero_input(size_t idx, Gate_t inp) {
    this->zero_gates.at(idx).add_input(inp);
}

void Multiplexer_t::set_one_input(size_t idx, Gate_t inp) {
    this->one_gates.at(idx).add_input(inp);
}

void Multiplexer_t::set_zero_input(std::vector<Gate_t> inp) {
    if(inp.size() != this->zero_gates.size())
        throw std::runtime_error("Multiplexer_t::set_zero_input : input width must match mux width");

    for(size_t i : range(0UL, inp.size()))
        this->set_zero_input(i, inp.at(i));
}

void Multiplexer_t::set_one_input(std::vector<Gate_t> inp) {
    if(inp.size() != this->zero_gates.size())
        throw std::runtime_error("Multiplexer_t::set_one_input : input width must match mux width");

    for(size_t i : range(0UL, inp.size()))
        this->set_one_input(i, inp.at(i));
}
