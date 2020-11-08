from __future__ import print_function

from Gate import *
from BitVector import *

from lib.modules.bcd_converter import *

inputs = [
    SIGNAL(),
    SIGNAL(),
    SIGNAL(),
    SIGNAL()
]

outputs = None

plus3 = BCD_Digit.Plus3()
plus3.set_input_vector(inputs)
outputs = plus3.get_output_vector()

for i in range(0, 16):

    inputs[0].set_output_value((i >> 0) & 1)
    inputs[1].set_output_value((i >> 1) & 1)
    inputs[2].set_output_value((i >> 2) & 1)
    inputs[3].set_output_value((i >> 3) & 1)

    hdl.logic_gate_simulate()

    calc = (i + 3) & 0x0F

    sim = (outputs[0].get_output_value() << 0)
    sim |= (outputs[1].get_output_value() << 1)
    sim |= (outputs[2].get_output_value() << 2)
    sim |= (outputs[3].get_output_value() << 3)

    print('sim: {0}, calc: {1}'.format(sim, calc))

    assert ((calc >> 0) & 0x01) == outputs[0].get_output_value()
    assert ((calc >> 1) & 0x01) == outputs[1].get_output_value()
    assert ((calc >> 2) & 0x01) == outputs[2].get_output_value()
    assert ((calc >> 3) & 0x01) == outputs[3].get_output_value()
