from __future__ import print_function

from Gate import *
from BitVector import *

from lib.modules.bcd_converter import *

digitadj = BCD_Digit.DigitAdjust()

# I/O
inputs  = [ SIGNAL(), SIGNAL(), SIGNAL(), SIGNAL() ]
outputs = digitadj.get_output_vector()

digitadj.set_input_vector(inputs)

for i in range(0, 16):

    for k in range(0, 4):
        inputs[k].set_output_value( (i >> k) & 0x01 )

    hdl.logic_gate_simulate()

    m = 0
    m |= (outputs[0].get_output_value() << 0)
    m |= (outputs[1].get_output_value() << 1)
    m |= (outputs[2].get_output_value() << 2)
    m |= (outputs[3].get_output_value() << 3)

    # BCD adjust stage only adjusts numbers greater than 4
    if i > 4:
        assert (( i + 3 ) & 0x0F) == m
    else:
        assert i == m
