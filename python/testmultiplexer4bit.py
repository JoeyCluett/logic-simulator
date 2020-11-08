from __future__ import print_function

from Gate import *
from BitVector import *

from lib.modules.multiplexer import *

mux4 = Multiplexer(select_bits=2)

inputs  = [ SIGNAL(), SIGNAL(), SIGNAL(), SIGNAL() ]
selects = [ SIGNAL(), SIGNAL() ]
output  = None

mux4.set_input_vector(inputs)
mux4.set_select_vector(selects)
output = mux4.get_output()

for i in range(0, 16):

    inputs[0].set_output_value((i >> 0) & 0x01)
    inputs[1].set_output_value((i >> 1) & 0x01)
    inputs[2].set_output_value((i >> 2) & 0x01)
    inputs[3].set_output_value((i >> 3) & 0x01)

    for j in range(0, 4):
        selects[0].set_output_value((j >> 0) & 0x01)
        selects[1].set_output_value((j >> 1) & 0x01)

        hdl.logic_gate_simulate()

        test_value = (i >> j) & 0x01

        assert test_value == output.get_output_value()

