from Gate import *

from multiplexer import *
from shiftregister import *

class BCD_Digit:

    class DigitCompare:
        def __init__(self):
            self.o_low = OR()
            self.a_mid = AND()
            self.o_hi  = OR()

            self.a_mid.add_input(self.o_low)
            self.o_hi.add_input(self.a_mid)

        def set_input_vector(self, vec):
            self.o_low.add_input(vec[0], vec[1])
            self.a_mid.add_input(vec[2])
            self.o_hi.add_input(vec[3])

        def get_output(self):
            return self.o_hi

    class Plus3:
        def __init__(self):
            self.n0 = NOR()
            self.n1 = NOR()
            self.x0 = XOR()
            self.x1 = XOR()
            self.x2 = XOR()
            self.a0 = AND()
            self.a1 = AND()
            self.o0 = OR()

            self.x0.add_input(self.n1)
            self.a0.add_input(self.n1)
            self.o0.add_input(self.a0)
            self.x1.add_input(self.o0)
            self.a1.add_input(self.o0)
            self.x2.add_input(self.a1)

        def set_input_vector(self, vec):

            self.n0.add_input(vec[0])
            self.x0.add_input(vec[0])
            self.a0.add_input(vec[0])

            self.n1.add_input(vec[1])
            self.o0.add_input(vec[1])

            self.x1.add_input(vec[2])
            self.a1.add_input(vec[2])

            self.x2.add_input(vec[3])

        def get_output_vector(self):
            outvec = [
                self.n0,
                self.x0,
                self.x1,
                self.x2
            ]

            return outvec

    class Selector:
        def __init__(self):

            self.sel_0 = [ AND(), AND(), AND(), AND() ]
            self.sel_1 = [ AND(), AND(), AND(), AND() ]
            self.or_g  = [ OR(),  OR(),  OR(),  OR() ]

            self.not_gate = NOR()

            for i in range(0, 4):
                self.sel_0[i].add_input(self.not_gate)            

            for i in range(0, 4):
                self.or_g[i].add_input(self.sel_0[i])
                self.or_g[i].add_input(self.sel_1[i])

        def set_input_vec_0(self, vec):
            for i in range(0, 4):
                self.sel_0[i].add_input(vec[i])

        def set_input_vec_1(self, vec):
            for i in range(0, 4):
                self.sel_1[i].add_input(vec[i])

        def set_select(self, sel):
            self.not_gate.add_input(sel)
            for i in range(0, 4):
                self.sel_1[i].add_input(sel)

        def get_output_vector(self):
            return self.or_g

    class DigitAdjust:
        def __init__(self):
            self.selector   = BCD_Digit.Selector()
            self.plus3      = BCD_Digit.Plus3()
            self.comparedig = BCD_Digit.DigitCompare()

            self.selector.set_input_vec_1(self.plus3.get_output_vector())
            self.selector.set_select(self.comparedig.get_output())

        def set_input_vector(self, vec):
            self.selector.set_input_vec_0(vec)
            self.plus3.set_input_vector(vec)
            self.comparedig.set_input_vector(vec)

        def get_output_vector(self):
            return self.selector.get_output_vector()

    def __init__(self):
        self.adj = BCD_Digit.DigitAdjust()

        self.ffs = [ FLIPFLOP(), FLIPFLOP(), FLIPFLOP(), FLIPFLOP() ]
        self.mux = [
            Multiplexer(select_bits=1),
            Multiplexer(select_bits=1),
            Multiplexer(select_bits=1),
            Multiplexer(select_bits=1)
        ]

        self.shift_in = FORWARD()

        adj_out = self.adj.get_output_vector()

        self.mux[0].set_input_vector([ self.shift_in, ZERO() ])
        self.mux[1].set_input_vector([ adj_out[0],    ZERO() ])
        self.mux[2].set_input_vector([ adj_out[1],    ZERO() ])
        self.mux[3].set_input_vector([ adj_out[2],    ZERO() ])

        for i in range(0, 4):
            self.ffs[i].set_data(self.mux[i].get_output())

        input_vector = [
            self.ffs[0],
            self.ffs[1],
            self.ffs[2],
            self.ffs[3]
        ]

        self.adj.set_input_vector(input_vector)

    def set_nShift_Reset(self, nshift_reset):
        for i in range(0, 4):
            self.mux[i].set_select_vector([ nshift_reset ])

    def set_clock(self, clk):
        for i in range(0, 4):
            self.ffs[i].set_clock(clk)

    def set_shift_in(self, shift_in):
        self.shift_in.add_input(shift_in)

    def get_shift_out(self):
        return self.adj.get_output_vector()[3]

    def get_output_vector(self):
        return self.ffs

    def __str__(self):
        return '{0}{1}{2}{3}'.format(
            self.ffs[3].get_output_value(),
            self.ffs[2].get_output_value(),
            self.ffs[1].get_output_value(),
            self.ffs[0].get_output_value())

    def to_dec(self):
        t = 0
        for i in range(0, 4):
            t |= ( ( self.ffs[i].get_output_value() & 0x01 ) << i)
        return t

class BCD_Converter:

    def __init__(self, bitwidth):

        ndigits = ceil(bitwdth / 3.0)

        self.bitwidth       = bitwidth
        self.input_register = piso_shiftregister(bitwidth)
        self.digits         = []

        for i in range(0, ndigits):
            self.digits.append(BCD_Digit())

        # link digits together
        self.digits[0].set_shift_in(self.input_register.get_shift_out())
        for k in range(1, ndigits):
            self.digits[k].set_shift_in(self.digits[k-1].get_shift_out())

    def set_clock(self, clk):
        for el in self.digits:
            el.set_clock(clk)

        self.input_register.set_clock(clk)

    def set_nShift_load(self, sig):
        for el in self.digits:
            el.set_nShift_Reset(sig)

        self.input_register.

    def get_decimal_result(self):
        n = ''
        for el in self.digits:
            n += str(el.to_dec())
        return n[::-1]



