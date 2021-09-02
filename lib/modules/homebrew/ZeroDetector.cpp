#include <lib/modules/homebrew/ZeroDetector.h>
#include <lib/cpp-api/range.h>

#include <stdexcept>
#include <vector>

ZeroDetector_t::ZeroDetector_t(void)
        : norgate(NOR()) {

    for(size_t i : range(0UL, 16)) {
        this->input_forwards[i].set(FORWARD());
        this->norgate.add_input(this->input_forwards[i]);
    }
}

Gate_t ZeroDetector_t::get_output(void) {
    return this->norgate;
}

void ZeroDetector_t::set_input(std::vector<Gate_t> gates) {
    if(gates.size() != 16)
        throw std::runtime_error("ZeroDetector_t::set_input : input width must be 16");

    for(size_t idx : range(0UL, 16))
        this->input_forwards[idx].add_input(gates.at(idx));
}
