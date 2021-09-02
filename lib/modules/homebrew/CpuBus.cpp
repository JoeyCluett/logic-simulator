#include <lib/modules/homebrew/CpuBus.h>
#include <lib/cpp-api/range.h>

#include <vector>
#include <stdexcept>

CpuBus_t::CpuBus_t(void) {
    for(int i : range(0, 16))
        this->bus[i].set(WIRE());
}

void CpuBus_t::connect_to_bus(std::vector<Gate_t> interface) {
    if(interface.size() != 16)
        throw std::runtime_error("DataBus_t::connect_to_bus : interface device must be 16 bits wide");

    for(int i : range(0, 16))
        this->bus[i].add_input(interface[i]);
}

std::vector<Gate_t> CpuBus_t::get_bus(void) {
    return { this->bus, this->bus+16 };
}


