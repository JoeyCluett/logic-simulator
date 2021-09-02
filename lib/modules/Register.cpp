#include <lib/modules/Register.h>
#include <lib/modules/BufferedFlipFlop.h>
#include <lib/cpp-api/range.h>
#include <vector>

Register_t::Register_t(size_t width) {
    this->flipflops.resize(width);
}

Gate_t Register_t::get_output(int idx) {
    return this->flipflops.at(idx).get_output();
}

std::vector<Gate_t> Register_t::get_output(void) {
    std::vector<Gate_t> vec;
    for(size_t idx : range(0UL, this->flipflops.size()))
        vec.push_back(this->flipflops.at(idx).get_output());
    return vec;
}

void Register_t::set_data(int idx, Gate_t gate) {
    this->flipflops.at(idx).set_data_input(gate);
}

void Register_t::set_data(std::vector<Gate_t> gates) {
    if(gates.size() != this->flipflops.size())
        throw std::runtime_error("Register_t::set_data : input width must match Register_t width");

    for(size_t idx : range(0UL, gates.size()))
        this->set_data(idx, gates.at(idx));
}

void Register_t::set_clock(Gate_t gate) {
    for(auto& ff : this->flipflops)
        ff.set_clock_input(gate);
}

void Register_t::set_enable(Gate_t gate) {
    for(auto& ff : this->flipflops)
        ff.set_enable_input(gate);
}


