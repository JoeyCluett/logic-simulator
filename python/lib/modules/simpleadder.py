
from Gate import *

class simpleadder:
    def __init__(self):
        self.a_lo = AND()
        self.x_lo = XOR()
        self.a_hi = AND()
        self.x_hi = XOR()

        self.or_gate = OR()

        self.a_hi.add_input(self.x_lo)
        self.x_hi.add_input(self.x_lo)
        self.or_gate.add_input(self.a_lo, self.a_hi)

    def set_carry_in(self, cin_gate):
        self.a_hi.add_input(cin_gate)
        self.x_hi.add_input(cin_gate)

    def set_a(self, a_gate):
        self.a_lo.add_input(a_gate)
        self.x_lo.add_input(a_gate)

    def set_b(self, b_gate):
        self.a_lo.add_input(b_gate)
        self.x_lo.add_input(b_gate)

    def get_carry_out(self):
        return self.or_gate

    def get_sum(self):
        return self.x_hi

