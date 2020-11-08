from Gate import *

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
            self.selector   = Selector()
            self.plus3      = Plus3()
            self.comparedig = DigitCompare()

            self.selector.set_input_vec_1(self.plus3.get_output_vector())
            self.selector.set_select(self.comparedig.get_output())

        def set_input_vector(self, vec):
            self.selector.set_input_vec_0(vec)
            self.plus3.set_input_vector(vec)
            self.comparedig.set_input_vector(vec)

        def get_output_vector(self):
            return self.selector.get_output_vector()

class BCD_Converter:

    def __init__(self):
        self.ffs = [
            FLIPFLOP(),
            FLIPFLOP(),
            FLIPFLOP(),
            FLIPFLOP()
        ]

        self.ffs[1].set_data(self.ffs[0])
        self.ffs[2].set_data(self.ffs[1])
        self.ffs[3].set_data(self.ffs[2])

    def get_shift_out(self):
        return self.ffs[3]

    def set_shift_in(self, g):
        self.ffs[0].set_data(g)



