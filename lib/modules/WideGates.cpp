#include <lib/modules/WideGates.h>

WideAnd_t::WideAnd_t(void)
        : and_gate(AND()) {
}

void WideAnd_t::set(std::vector<Gate_t> gates) {
    for(auto& g : gates)
        this->and_gate.add_input(g);
}

Gate_t WideAnd_t::get_output(void) {
    return this->and_gate;
}


