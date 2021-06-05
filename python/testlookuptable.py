from __future__ import print_function

from Gate import *
from BitVector import *
from LookupTable import *

def signal_vec(width):
    tmp = []
    for i in range(0, width):
        tmp.append(SIGNAL())

    return tmp

def signal_vec_set(value, vec):
    for i in range(0, len(vec)):
        vec[i].set_output_value( (value >> i) & 0x01 )

def signal_vec_get(vec):

    v = int(0)

    for i in range(0, len(vec)):
        sig_val = vec[i].get_output_value()
        if(sig_val == 1):
            v += pow(2, i)

    return int(v)

class lut_adder:

    def __init__(self, bit_width):
        self.bit_width = bit_width
        
        self.lut_arr = []
        for i in range(0, bit_width):
            self.lut_arr.append( LookupTable(input_width=3, output_width=2) )
            self.lut_arr[i].set_io_map([
                #   in    out
                #  ABCi   SC
                ( '000', '00' ),
                ( '001', '10' ),
                ( '010', '10' ),
                ( '011', '01' ),
                ( '100', '10' ),
                ( '101', '01' ),
                ( '110', '01' ),
                ( '111', '11' )
            ])
        
        # set internal carries
        for i in range(1, bit_width):
            self.lut_arr[i].set_input_idx(2, self.lut_arr[i-1].get_output()[1])

    def get_sum_idx(self, idx):
        assert idx >= 0 and idx < self.bit_width
        return self.lut_arr[idx].get_output()[0]

    def get_carry_out_idx(self, idx):
        assert idx >= 0 and idx < self.bit_width
        return self.lut_arr[idx].get_output()[1]

    def get_sum(self):
        sum_arr = []
        for el in self.lut_arr:
            sum_arr.append( el.get_output()[0] )
        return sum_arr

    def set_a(self, a):
        assert len(a) == self.bit_width
        for i in range(0, len(a)):
            self.lut_arr[i].set_input_idx(0, a[i])

    def set_b(self, b):
        assert len(b) == self.bit_width
        for i in range(0, len(b)):
            self.lut_arr[i].set_input_idx(1, b[i])

    def get_carry_out(self):
        return self.lut_arr[ self.bit_width - 1 ].get_output()[1]

    def set_carry_in(self, gate):
        self.lut_arr[0].set_input_idx(2, gate)


