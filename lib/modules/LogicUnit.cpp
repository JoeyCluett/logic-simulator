#include <lib/modules/LogicUnit.h>

#include <vector>
#include <lib/cpp-api/range.h>
#include <assert.h>

// ========================================================
// LogicUnit_t implementation
// ========================================================

LogicUnit_t::LogicUnit_t(size_t width) {
    this->logic_slices.resize(width);
}

void LogicUnit_t::set_a(size_t idx, Gate_t gate) {
    this->logic_slices.at(idx).set_a(gate);
}

void LogicUnit_t::set_b(size_t idx, Gate_t gate) {
    this->logic_slices.at(idx).set_b(gate);
}

void LogicUnit_t::set_a(std::vector<Gate_t> gates) {
    if(gates.size() != this->logic_slices.size())
        throw std::runtime_error("LogicUnit_t::set_a : input width must match logic unit width");

    for(size_t i : range(0UL, gates.size()))
        this->set_a(i, gates.at(i));
}

void LogicUnit_t::set_b(std::vector<Gate_t> gates) {
    if(gates.size() != this->logic_slices.size())
        throw std::runtime_error("LogicUnit_t::set_b : input width must match logic unit width");

    for(size_t i : range(0UL, gates.size()))
        this->set_b(i, gates.at(i));
}

Gate_t LogicUnit_t::get_output(size_t idx) {
    return this->logic_slices.at(idx).get_output();
}

std::vector<Gate_t> LogicUnit_t::get_output_vector(void) {
    
    std::vector<Gate_t> vec;

    for(auto& ls : this->logic_slices)
        vec.push_back(ls.get_output());

    return vec;
}

// ========================================================
// LogicSlice_t implementation
// ========================================================

LogicSlice_t::LogicSlice_t(void) : 
        and_gate(AND()), nand_gate(NAND()), 
        or_gate(OR()),   nor_gate(NOR()), 
        xor_gate(XOR()), xnor_gate(XNOR()),
        output_wire(WIRE()) {

    for(int i : range(0, 6)) {
        this->z_bufs[i].set(TRISTATE_BUFFER());
        this->output_wire.add_input(this->z_bufs[i]);
    }

    this->z_bufs[0].tristate_set_data(this->and_gate);
    this->z_bufs[1].tristate_set_data(this->nand_gate);
    this->z_bufs[2].tristate_set_data(this->or_gate);
    this->z_bufs[3].tristate_set_data(this->nor_gate);
    this->z_bufs[4].tristate_set_data(this->xor_gate);
    this->z_bufs[5].tristate_set_data(this->xnor_gate);
}

Gate_t LogicSlice_t::get_output(void) {
    return this->output_wire;
}

void LogicSlice_t::set_a(Gate_t a) {
    this->and_gate.add_input(a);
    this->nand_gate.add_input(a);
    this->or_gate.add_input(a);
    this->nor_gate.add_input(a);
    this->xor_gate.add_input(a);
    this->xnor_gate.add_input(a);
}

void LogicSlice_t::set_b(Gate_t b) {
    this->set_a(b);
}

void LogicSlice_t::Test(void) {

    {
        LogicSlice_t logicslice;

    }

}
