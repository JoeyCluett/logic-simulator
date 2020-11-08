from __future__ import print_function

from Gate import *
from BitVector import *

from lib.modules.bcd_converter import *

bcdsel = BCD_Digit.Selector()

sel = SIGNAL()
input_0 = [ SIGNAL(), SIGNAL(), SIGNAL(), SIGNAL() ]
input_1 = [ SIGNAL(), SIGNAL(), SIGNAL(), SIGNAL() ]
outputs = None

bcdsel.set_input_vec_0(input_0)
bcdsel.set_input_vec_1(input_1)
bcdsel.set_select(sel)
outputs = bcdsel.get_output_vector()

for i in range(0, 16):

    input_0[0].set_output_value(( i >> 0 ) & 0x01)
    input_0[1].set_output_value(( i >> 1 ) & 0x01)
    input_0[2].set_output_value(( i >> 2 ) & 0x01)
    input_0[3].set_output_value(( i >> 3 ) & 0x01)

    j = i + 1

    input_1[0].set_output_value(( j >> 0 ) & 0x01)
    input_1[1].set_output_value(( j >> 1 ) & 0x01)
    input_1[2].set_output_value(( j >> 2 ) & 0x01)
    input_1[3].set_output_value(( j >> 3 ) & 0x01)

    sel.set_output_value(0)
    hdl.logic_gate_simulate()
    assert outputs[0].get_output_value() == (( i >> 0 ) & 0x01)
    assert outputs[1].get_output_value() == (( i >> 1 ) & 0x01)
    assert outputs[2].get_output_value() == (( i >> 2 ) & 0x01)
    assert outputs[3].get_output_value() == (( i >> 3 ) & 0x01)

    sel.set_output_value(1)
    hdl.logic_gate_simulate()
    assert outputs[0].get_output_value() == (( j >> 0 ) & 0x01)
    assert outputs[1].get_output_value() == (( j >> 1 ) & 0x01)
    assert outputs[2].get_output_value() == (( j >> 2 ) & 0x01)
    assert outputs[3].get_output_value() == (( j >> 3 ) & 0x01)
