import hdl

class GateType:
    logic_gate_and  = 0
    logic_gate_nand = 1
    logic_gate_or   = 2
    logic_gate_nor  = 3
    logic_gate_xor  = 4
    logic_gate_xnor = 5

    logic_gate_flipflop = 6
    logic_gate_none     = 7
    logic_gate_constant = 8
    logic_gate_signal   = 9
    logic_gate_forward  = 10


class AllGate:
    def __init__(self):
        self.ptr = None

    def get_type(self):
        return hdl.logic_gate_get_type(self.ptr)

    def get_output_value(self):
        return hdl.logic_gate_get_output_value(self.ptr)

    def get_num_inputs(self):
        return hdl.logic_gate_get_num_inputs(self.ptr)

class RegularGate(AllGate):
    def __init__(self, _type):
        AllGate.__init__(self)
        ptr = hdl.logic_gate_alloc()
        self.ptr = hdl.logic_gate_init(ptr, _type)

    def add_input(self, *args):
        for g in args:
            hdl.logic_gate_add_input(self.ptr, g.ptr)

    def add_inv_input(self, *args):
        for g in args:
            not_gate = NOR()
            not_gate.add_input(g)
            hdl.logic_gate_add_input(self.ptr, not_gate.ptr)

# kind of a weird gate
class FORWARD:
    def __init__(self):
        self.ptr = hdl.logic_gate_new(GateType.logic_gate_forward)

    def add_input(self, gate):
        hdl.logic_gate_add_input(self.ptr, gate.ptr)

    def add_inv_input(self, gate):
        not_gate = NOR()
        not_gate.add_input(gate)
        hdl.logic_gate_add_input(self.ptr, not_gate.ptr)


# ===============================================
# all lowest level gate types
# ===============================================

class FLIPFLOP(AllGate):
    def __init__(self):
        AllGate.__init__(self)
        ptr = hdl.logic_gate_alloc()
        self.ptr = hdl.logic_gate_init(ptr, GateType.logic_gate_flipflop)

    def set_clock(self, g):
        hdl.logic_gate_flipflop_set_clock(self.ptr, g.ptr)

    def set_data(self, g):
        hdl.logic_gate_flipflop_set_data(self.ptr, g.ptr)

class SIGNAL(AllGate):
    def __init__(self):
        AllGate.__init__(self)
        self.ptr = hdl.logic_gate_new(GateType.logic_gate_signal)

    def set_output_value(self, value):
        hdl.logic_gate_signal_set(self.ptr, value)

class ZERO(AllGate):
    def __init__(self):
        AllGate.__init__(self)
        self.ptr = hdl.logic_gate_get_constant_zero()

class ONE(AllGate):
    def __init__(self):
        AllGate.__init__(self)
        self.ptr = hdl.logic_gate_get_constant_one()

class AND(RegularGate):
    def __init__(self, *args):
        RegularGate.__init__(self, GateType.logic_gate_and)
        for g in args:
            self.add_input(g)

class NAND(RegularGate):
    def __init__(self, *args):
        RegularGate.__init__(self, GateType.logic_gate_nand)
        for g in args:
            self.add_input(g)

class OR(RegularGate):
    def __init__(self, *args):
        RegularGate.__init__(self, GateType.logic_gate_or)
        for g in args:
            self.add_input(g)

class NOR(RegularGate):
    def __init__(self, *args):
        RegularGate.__init__(self, GateType.logic_gate_nor)
        for g in args:
            self.add_input(g)

class XOR(RegularGate):
    def __init__(self, *args):
        RegularGate.__init__(self, GateType.logic_gate_xor)
        for g in args:
            self.add_input(g)

class XNOR(RegularGate):
    def __init__(self, *args):
        RegularGate.__init__(self, GateType.logic_gate_xnor)
        for g in args:
            self.add_input(g)
