
require("simload");
require("gate");

require("Multiplexer");
require("Selector_NBit");
require("Adder_CLA_4Bit");

BCD_Generator_NBit = {}

BCD_Generator_NBit.new = function(bitwidth)

    -- define a 4 bit shift register with some extras
    local BCD_Digit = {};
    BCD_Digit.new = function()
        local self = {};

        self.reg   = {};
        self.muxes = {};

        for i=0,3 do
            self.reg[i]   = Gate.FLIPFLOP();

            -- configure mux for this flip flop input
            self.muxes[i] = Multiplexer.new(2);
            self.muxes[i].set_Input(2, Gate.CONSTANT(0));
            self.muxes[i].set_Input(3, Gate.CONSTANT(0));

            self.reg[i].set_flipflop_data(self.muxes[i].get_Output());

        end

        -- flip flops are chained together internally via muxes
        self.muxes[1].set_Input(1, self.reg[0]);
        self.muxes[2].set_Input(1, self.reg[1]);
        self.muxes[3].set_Input(1, self.reg[2]);

        local adder    = Adder_CLA_4Bit.new();
        local selector = Selector_NBit.new(4);

        local greater_than_4 = 
            Gate.OR(
                self.reg[3],
                Gate.AND(
                    self.reg[2],
                    Gate.OR(
                        self.reg[0], 
                        self.reg[1]
                    )));

        selector.set_Select(greater_than_4);

        -- one input is always 3
        adder.set_B(0, Gate.CONSTANT(1));
        adder.set_B(1, Gate.CONSTANT(1));
        adder.set_B(2, Gate.CONSTANT(0));
        adder.set_B(3, Gate.CONSTANT(0));

        adder.set_CarryIn(Gate.CONSTANT(0));

        -- flip flop outputs are Port A
        for i=0,3 do adder.set_A(i, self.reg[i]); end

        -- outputs of adder goes into selector inputs (1)
        for i=0,3 do selector.set_Input1(i, adder.get_Sum(i)) end

        -- flip flop outputs go into selector inputs (0)
        for i=0,3 do selector.set_Input0(i, self.reg[i]) end

        -- selector output feeds back into muxes for flip flops
        for i=0,3 do self.muxes[i].set_Input(0, selector.get_Output(i)) end

        -- ======================================
        -- interface methods
        -- ======================================

        self.set_ShiftIn  = function(gate) self.muxes[0].set_Input(1, gate); end
        self.get_ShiftOut = function() return self.reg[3]; end

        self.set_Clock = function(gate) 
            for i=0,3 do 
                self.reg[i].set_flipflop_clock(gate); 
            end 
        end

        self.set_NShift_Feedback = function(gate)
            for i=0,3 do
                self.muxes[i].set_Select(0, gate);
            end
        end

        self.set_ResetInput = function(gate)
            for i=0,3 do
                self.muxes[i].set_Select(1, gate);
            end
        end

        return self;
    end

    local self = {};

    self.bcd_digits = {};
    local n_digits = math.ceil((bitwidth * 1.0) / 3.0);
    self.num_digits = n_digits;

    for i=0,(n_digits-1) do
        self.bcd_digits[i] = BCD_Digit.new();
    end

    

    return self;
end

