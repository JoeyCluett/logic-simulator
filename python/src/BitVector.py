#
# utility functions for lists of Gate types
#

from Gate import *

def set_vec_to_value(vec, value):
    for i in range(len(vec)):
        vec[i].set_output_value( (value >> i) & 0x01 )

def new_signal_vec(veclen):
    v = []

    for i in range(veclen):
        v.append(SIGNAL())

    return v

#
# duck typing to the rescue
#
class BitVecValue:
    def __init__(self, value):
        self.value = value

    def get_output_value(self):
        return self.value

    def set_output_value(self, value):
        self.value = value

#
# simple wrapper over an array of SIGNAL types
#
class SignalVector:
    def __init__(self, length):
        self.signals = []

        for i in range(0, length):
            self.signals.append(SIGNAL())

    def set_value(self, idx, value):
        self.signals[idx].set_value(value)

    def get_output_value(self, idx):
        return self.signals[idx].get_output_value()

def bitvector_equal(vec_a, vec_b):
    if len(vec_a) != len(vec_b):
        raise Exception('bitvector_equal : input vectors must be same length')

    for i in range(0, len(vec_a)):
        if vec_a[i].get_output_value() != vec_b[i].get_output_value():
            return False

    return True

def bitvector_increment(vec):

    for el in vec:

        if el.get_output_value() == 1:
            el.set_output_value(0)

        elif el.get_output_value() == 0:
            el.set_output_value(1)
            return

def bitvector_add(vec_a, vec_b, carry_in):

    if len(vec_a) != len(vec_b):
        raise Exception('bitvector_add : input vectors must be same length')

    carry_in_value = carry_in.get_output_value()
    result = []

    for i in range(0, len(vec_a)):
        a_val = vec_a[i].get_output_value()
        b_val = vec_b[i].get_output_value()

        res = a_val + b_val + carry_in_value
        result.append(BitVecValue(res & 1))

        carry_in_value = (res >> 1) & 1

    result.append(BitVecValue(carry_in_value))

    return result
