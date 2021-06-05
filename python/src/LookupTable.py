#
# generate a lookup table
#

from Gate import *

class LookupTable:

    def __init__(self, input_width, output_width):
        self.input    = []
        self.n_input  = []
        self.output   = []

        self.input_width  = input_width
        self.output_width = output_width

        for i in range(0, input_width):
            self.input.append(FORWARD())

            self.n_input.append(NOR())
            self.n_input[i].add_input(self.input[i])
            
        for i in range(0, output_width):
            self.output.append(OR())

    def set_input(self, input_list):
        assert len(input_list) == len(self.input)

        for i in range(0, self.input_width):
            self.input[i].add_input(input_list[i])

    def set_input_idx(self, idx, gate):
        assert idx >= 0 and idx < self.input_width
        self.input[idx].add_input(gate)

    def get_output(self):
        return self.output

    def set_io_map(self, io_list):

        for el in io_list:

            assert len(el) == 2, '{0}'.format(el)
            assert len(el[0]) == self.input_width, '{0}'.format(el[0])
            assert len(el[1]) == self.output_width, '{0}'.format(el[1])

            and_gate = AND()

            for i in range(0, len(el[0])): # input bits
                cur_el = el[0][i]

                if   cur_el == '0': 
                    and_gate.add_input(  self.n_input[i] )
                elif cur_el == '1': 
                    and_gate.add_input(  self.input[i] )
                else:
                    raise Exception('LookupTable.set_io_map : invalid input bit \'{0}\''.format(cur_el))

            for i in range(0, len(el[1])): # output bits
                cur_el = el[1][i]

                if cur_el == '1': 
                    self.output[i].add_input(and_gate)
                elif cur_el != '0':
                    raise Exception('LookupTable.set_io_map : invalid output bit \'{0}\''.format(cur_el))

