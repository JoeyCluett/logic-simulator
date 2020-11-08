from __future__ import print_function

from Gate import *
from BitVector import *

from lib.modules.multiplexer import *

mux16 = Multiplexer(select_bits=4)

inputs  = []
selects = [ SIGNAL(), SIGNAL(), SIGNAL(), SIGNAL() ]
output  = None

for i in range(0, 16):
    inputs.append(SIGNAL())

mux16.set_input_vector(inputs)
mux16.set_select_vector(selects)
output = mux16.get_output()

for i in range(0, pow(2, 16)):

    for j in range(0, 16):
        inputs[j].set_output_value( (i >> j) & 0x01 )

    for j in range(0, 16):
        selects[0].set_output_value( (j >> 0) & 0x01 )
        selects[1].set_output_value( (j >> 1) & 0x01 )
        selects[2].set_output_value( (j >> 2) & 0x01 )
        selects[3].set_output_value( (j >> 3) & 0x01 )

        hdl.logic_gate_simulate()

        test_value = (i >> j) & 0x01

        assert test_value == output.get_output_value()
