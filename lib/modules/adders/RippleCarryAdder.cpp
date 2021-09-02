#include <lib/modules/adders/RippleCarryAdder.h>
#include <lib/cpp-api/range.h>

#include <vector>
#include <stdexcept>

RippleCarryAdder_t::RippleCarryAdder_t(size_t width) {
    this->adders.resize(width);

    // create ripple carry chain
    for(size_t i = 1; i < width; i++)
        this->adders.at(i).set_ci( this->adders.at(i-1).get_co() );
}

void RippleCarryAdder_t::set_a(std::vector<Gate_t> inp) {
    if(inp.size() != this->adders.size())
        throw std::runtime_error(
            "RippleCarryAdder_t::set_a : input vector size must match adder width");

    for(size_t i : range(0UL, inp.size()))
        this->set_a(i, inp.at(i));
}

void RippleCarryAdder_t::set_b(std::vector<Gate_t> inp) {
    if(inp.size() != this->adders.size())
        throw std::runtime_error(
            "RippleCarryAdder_t::set_b : input vector size must match adder width");

    for(size_t i : range(0UL, inp.size()))
        this->set_b(i, inp.at(i));
}

void RippleCarryAdder_t::set_a(int idx, Gate_t inp) {
    this->adders.at(idx).set_a(inp);
}

void RippleCarryAdder_t::set_b(int idx, Gate_t inp) {
    this->adders.at(idx).set_b(inp);
}

void RippleCarryAdder_t::set_ci(Gate_t inp) {
    this->adders.at(0).set_ci(inp);
}

Gate_t RippleCarryAdder_t::get_sum(int idx) {
    return this->adders.at(idx).get_sum();
}

std::vector<Gate_t> RippleCarryAdder_t::get_sum(void) {
    std::vector<Gate_t> sum;
    for(auto& adder : this->adders) {
        sum.push_back(adder.get_sum());
    }
    return sum;
}

Gate_t RippleCarryAdder_t::get_co(void) {
    return this->adders.back().get_co();
}
