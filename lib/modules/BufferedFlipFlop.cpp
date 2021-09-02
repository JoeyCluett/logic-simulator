#include <lib/modules/BufferedFlipFlop.h>

BufferedFlipFlop_t::BufferedFlipFlop_t(void) 
        : flipflop(FLIPFLOP()), buffer(TRISTATE_BUFFER()) {
    this->buffer.tristate_set_data(this->flipflop);
}

Gate_t BufferedFlipFlop_t::get_output(void) {
    return this->buffer;
}

void BufferedFlipFlop_t::set_data_input(Gate_t gate) {
    this->flipflop.ff_set_data(gate);
}

void BufferedFlipFlop_t::set_clock_input(Gate_t gate) {
    this->flipflop.ff_set_clock(gate);
}

void BufferedFlipFlop_t::set_enable_input(Gate_t gate) {
    this->buffer.tristate_set_enable(gate);
}

