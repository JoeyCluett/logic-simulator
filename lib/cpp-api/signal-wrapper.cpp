#include <lib/cpp-api/signal-wrapper.h>
#include <lib/cpp-api/gate-wrapper.h>
#include <lib/logic.h>

#include <stdexcept>
#include <algorithm>

// =============================================================
// Signal_t implementation
// =============================================================

Signal_t::Signal_t(void) 
        : internal_gate(SIGNAL()) {
    this->internal_gate->output_value = OUTPUT_ZERO;
}

Signal_t::Signal_t(int initial_value)
        : internal_gate(SIGNAL()) {
    this->internal_gate->output_value = initial_value;
}

void Signal_t::set(int value) {
    this->internal_gate->output_value = value;
}

Signal_t::operator Gate_t() {
    return Gate_t(this->internal_gate);
}

Signal_t::operator logic_gate_ptr_t() {
    return this->internal_gate;
}

Signal_t::~Signal_t() {
    free(this->internal_gate);
}

// =============================================================
// SignalVector_t implementation
// =============================================================

SignalVector_t::SignalVector_t(size_t width) {
    this->signals.resize(width);
}

SignalVector_t::operator std::vector<Gate_t>() {
    std::vector<Gate_t> vec;
    for(auto& sig : this->signals)
        vec.push_back(Gate_t(sig.internal_gate));
    return vec;
}

SignalVector_t::operator std::vector<logic_gate_t*>() {
    std::vector<logic_gate_t*> vec;
    for(auto& sig : this->signals)
        vec.push_back(sig.internal_gate);
    return vec;
}

void SignalVector_t::set_value(size_t i) {


    {
        size_t rem = i >> this->signals.size();
        if(rem)
            throw std::runtime_error(
                    std::string(__PRETTY_FUNCTION__) + " : bit value '" + 
                    std::to_string(i) + "' exceeds length of internal buffer space");
    }

    for(size_t s = 0UL; s < this->signals.size(); s++) {
        size_t bit_value = i & 0x01;

        switch(bit_value) {
            case 0:
                this->signals.at(s).internal_gate->output_value = 0;
                break;
            case 1:
                this->signals.at(s).internal_gate->output_value = 1;
                break;
            default:
                throw std::runtime_error(
                        std::string(__PRETTY_FUNCTION__) + " : unknown internal error");
        }
    }

}

void SignalVector_t::set_value(const std::string& i) {
    if(i.size() > this->signals.size())
        throw std::runtime_error(
                std::string(__PRETTY_FUNCTION__) + " : bit value '" + i + 
                "' exceeds length of internal buffer space. size of given bitstring = '" + 
                std::to_string(i.size()) + "'");

    for(size_t s = 0UL; s < i.size(); s++) {
        const char c = i.at(s);

        switch(c) {
            case '0':
                this->signals.at(s).internal_gate->output_value = 0;
                break;
            case '1':
                this->signals.at(s).internal_gate->output_value = 1;
                break;
            default:
                throw std::runtime_error(
                        std::string(__PRETTY_FUNCTION__) + " : invalid char '" + 
                        std::to_string(c) + "' in input string '" + i + "'"
                );
        }
    }

}

void SignalVector_t::set_value_r(std::string i) {
    std::reverse(i.begin(), i.end());
    this->set_value(i);
}
