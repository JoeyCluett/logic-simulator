from Gate import *

class Multiplexer:

    def __init__(self, select_bits):

        self.and_g = []
        self.or_g = OR()
        self.invert_bits  = []
        self.not_inv_bits = []

        for i in range(0, select_bits):
            self.not_inv_bits.append(FORWARD())
            self.invert_bits.append(NOR())
            self.invert_bits[i].add_input(self.not_inv_bits[i])

        for i in range(0, pow(2, select_bits)):
            self.and_g.append(AND())
            
            # set inverted/non-inverted bits for each gate
            for j in range(0, select_bits):
                bitv = (i >> j) & 0x01

                if bitv == 0:
                    self.and_g[i].add_input(self.invert_bits[j])
                else:
                    self.and_g[i].add_input(self.not_inv_bits[j])

            self.or_g.add_input(self.and_g[i])

    def set_input_vector(self, vec):
        assert len(vec) == len(self.and_g)

        for i in range(0, len(vec)):
            self.and_g[i].add_input(vec[i])

    def set_select_vector(self, vec):
        assert len(vec) == len(self.not_inv_bits)

        for i in range(0, len(vec)):
            self.not_inv_bits[i].add_input(vec[i])

    def get_output(self):
        return self.or_g


