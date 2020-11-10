from Gate import *

from multiplexer import *
#
# serial-in parallel-out
#
class sipo_shiftregister:

    def __init__(self, length):
        self.length = length
        self.ff = []

        # allocate flipflop gates
        for i in range(0, length):
            self.ff.append(FLIPFLOP())

        # output(ff[i-1]) -> data(ff[i])
        for i in range(1, length):
            self.ff[i].set_data(self.ff[i-1])

    def set_clock(self, clk):
        for el in self.ff:
            el.set_clock(clk)

    def set_data_in(self, data):
        self.ff[0].set_data(data)

    def get_data_out(self, idx):
        return self.ff[idx]

    def get_output_value(self, idx):
        return self.ff[idx].get_output_value()

    def get_output_vector(self):
        return self.ff

#
# parallel-in serial-out
#
class piso_shiftregister:

    def __init__(self, length):
        self.length = length
        
        self.ff  = []
        self.mux = []

        self.input_forward = []

        for i in range(length):
            self.ff.append(FLIPFLOP())
            self.mux.append(Multiplexer(select_bits=1))
            self.ff[i].set_data(self.mux[i].get_output())
            self.input_forward.append(FORWARD())

        self.mux[0].set_input_vector([ ZERO(), self.input_forward[0] ])

        for i in range(1, length):
            self.mux[i].set_input_vector([ self.ff[i-1], self.input_forward[i] ])

    def set_input_vector(self, vec):
        assert len(vec) == len(self.input_forward)

        for i in range(0, len(vec)):
            self.input_forward[i].add_input(vec[i])

    def get_shift_out(self):
        return self.ff[-1]

    def set_clock(self, clk):
        for el in self.ff:
            el.set_clock(clk)
