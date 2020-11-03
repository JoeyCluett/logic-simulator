from Gate import *

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
