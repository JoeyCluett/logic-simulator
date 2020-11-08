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

digitcmp = BCD_Digit.DigitCompare()
digitcmp.set_input_vector(inputs)

output = digitcmp.get_output()

for i in range(0, 16):
    inputs[0].set_output_value((i >> 0) & 1)
    inputs[1].set_output_value((i >> 1) & 1)
    inputs[2].set_output_value((i >> 2) & 1)
    inputs[3].set_output_value((i >> 3) & 1)

    hdl.logic_gate_simulate()

    #print("{0} {1}".format(output.get_output_value(), i))

    if i <= 4:
        assert output.get_output_value() == 0
    else:
        assert output.get_output_value() == 1


