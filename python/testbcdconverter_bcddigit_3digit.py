from __future__ import print_function

from Gate import *
from BitVector import *

from lib.modules.bcd_converter import *

def print_digits(digits):

    dlen = len(digits)

    for i in range(0, dlen):
        print('{0}'.format(digits[dlen - 1 - i].to_dec()), end='')

    print(' = ', end='')

    for i in range(0, dlen):
        print('{0} '.format(str(digits[dlen - 1 - i])), end='')

    print()

def get_decimal_number(digits):
    dlen = len(digits)
    s = ''
    for i in range(0, dlen):
        s += str(digits[dlen - 1 - i].to_dec())
    return s

digits = [
    BCD_Digit(), # ones
    BCD_Digit(), # tens
    BCD_Digit()  # hundreds
]

#
#print('total gates allocated: {0}'.format(hdl.logic_allocator_ngates()))
#

clk          = SIGNAL()
nshift_reset = SIGNAL()
shift_in     = SIGNAL()

for i in range(0, 3):
    digits[i].set_clock(clk)
    digits[i].set_nShift_Reset(nshift_reset)

digits[0].set_shift_in(shift_in)
digits[1].set_shift_in(digits[0].get_shift_out())
digits[2].set_shift_in(digits[1].get_shift_out())

# assume 8-bit number
def eval_number(n):

    # reset digit output
    nshift_reset.set_output_value(1)
    clk.set_output_value(0)
    hdl.logic_gate_simulate()
    clk.set_output_value(1)
    hdl.logic_gate_simulate()

    nshift_reset.set_output_value(0)
    hdl.logic_gate_simulate()

    #print_digits(digits)

    for i in range(7, -1, -1):

        # shift each bit individually
        shift_in.set_output_value(( n >> i ) & 0x01)
        clk.set_output_value(0)
        hdl.logic_gate_simulate()
        clk.set_output_value(1)
        hdl.logic_gate_simulate()
        clk.set_output_value(0)
        hdl.logic_gate_simulate()

    #print_digits(digits)

for i in range(0, 256):
    eval_number(i)
    assert str(get_decimal_number(digits)) == str(i).zfill(3)


