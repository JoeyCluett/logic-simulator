from __future__ import print_function

from Gate import *
from BitVector import *

from lib.modules.bcd_converter import *

digit = BCD_Digit()

output = digit.get_output_vector()

clk          = SIGNAL()
nshift_reset = SIGNAL()
shift_in     = SIGNAL()

digit.set_clock(clk)
digit.set_nShift_Reset(nshift_reset)
digit.set_shift_in(shift_in)

# reset digit output
nshift_reset.set_output_value(1)
clk.set_output_value(0)
hdl.logic_gate_simulate()
clk.set_output_value(1)
hdl.logic_gate_simulate()

print('start: {0}'.format(str(digit)))
nshift_reset.set_output_value(0)
hdl.logic_gate_simulate()

# shift each bit individually

shift_in.set_output_value(0)
clk.set_output_value(0)
hdl.logic_gate_simulate()
clk.set_output_value(1)
hdl.logic_gate_simulate()

shift_in.set_output_value(1)
clk.set_output_value(0)
hdl.logic_gate_simulate()
clk.set_output_value(1)
hdl.logic_gate_simulate()

shift_in.set_output_value(1)
clk.set_output_value(0)
hdl.logic_gate_simulate()
clk.set_output_value(1)
hdl.logic_gate_simulate()

shift_in.set_output_value(1)
clk.set_output_value(0)
hdl.logic_gate_simulate()
clk.set_output_value(1)
hdl.logic_gate_simulate()

print('start: {0}'.format(str(digit)))

