
require("simload");
require("gate");

Adder_74283 = {}

Adder_74283.new = function()

    print("Adder_74283.new");

    function input_chunk()

        local self = {};

        self.g_nand = Gate.NAND();
        self.g_nor  = Gate.NOR();

        self.set_A = function(gate)
            self.g_nand.add_input(gate);
            self.g_nor.add_input(gate);
        end

        self.set_B = function(gate)
            self.g_nand.add_input(gate);
            self.g_nor.add_input(gate);
        end

        self.get_Carry = function()
            return self.g_nand;
        end

        self.get_Sum = function()
            return self.g_nor;
        end

        return self;
    end

    local self = {};

    self.inputs = {
        input_chunk(),
        input_chunk(),
        input_chunk(),
        input_chunk()
    };

    self.CO_nor = Gate.NOR();
    self.CI_not = Gate.NOR();

    self.Sum4_xor = Gate.XOR();
    self.Sum3_xor = Gate.XOR();
    self.Sum2_xor = Gate.XOR();
    self.Sum1_xor = Gate.XOR();

    -- ====================================================
    -- Interface methods
    -- ====================================================

    self.get_Sum = function(idx)
        if idx < 1 or idx > 4 then
            error("Adder_74283.get_Sum() : index out of range");
        end

        if idx == 1     then return self.Sum1_xor;
        elseif idx == 2 then return self.Sum2_xor;
        elseif idx == 3 then return self.Sum3_xor;
        elseif idx == 4 then return self.Sum4_xor;
        end

    end

    self.get_CarryOut = function()
        return self.CO_nor;
    end

    self.set_A = function(idx, gate)
        if idx < 1 or idx > 4 then
            error("Adder_74283.set_A() : index out of range")
        end
        self.inputs[idx].set_A(gate);
    end

    self.set_B = function(idx, gate)
        if idx < 1 or idx > 4 then
            error("Adder_74283.set_B() : index out of range")
        end
        self.inputs[idx].set_B(gate);
    end

    self.set_CarryIn = function(gate)
        self.CI_not.add_input(gate);
    end

    -- convenient wrappers over input names

    local i1_c = self.inputs[1].get_Carry();
    local i2_c = self.inputs[2].get_Carry();
    local i3_c = self.inputs[3].get_Carry();
    local i4_c = self.inputs[4].get_Carry();

    local i1_s = self.inputs[1].get_Sum();
    local i2_s = self.inputs[2].get_Sum();
    local i3_s = self.inputs[3].get_Sum();
    local i4_s = self.inputs[4].get_Sum();

    -- ====================================================
    -- Carry Out logic
    -- ====================================================
    
    self.CO_nor.add_input(self.inputs[4].get_Sum());
    
    local C4_and_0 = Gate.AND();
    local C4_and_1 = Gate.AND();
    local C4_and_2 = Gate.AND();
    local C4_and_3 = Gate.AND();
    
    Gate.add_input_list(C4_and_0, { i3_s, i4_c });
    Gate.add_input_list(C4_and_1, { i2_s, i4_c, i3_c });
    Gate.add_input_list(C4_and_2, { i1_s, i4_c, i3_c, i2_c });
    Gate.add_input_list(C4_and_3, { i4_c, i3_c, i2_c, i1_c, self.CI_not });

    Gate.add_input_list(self.CO_nor, { C4_and_0, C4_and_1, C4_and_2, C4_and_3 });

    -- ====================================================
    -- Sum-4 logic
    -- ====================================================
    
    local S4_and_0 = Gate.AND();
    S4_and_0.add_input(i4_c);
    S4_and_0.add_inv_input(i4_s);

    local S4_and_1 = Gate.AND();
    local S4_and_2 = Gate.AND();
    local S4_and_3 = Gate.AND();

    Gate.add_input_list(S4_and_1, { i2_s, i3_c });
    Gate.add_input_list(S4_and_2, { i1_s, i3_c, i2_c });
    Gate.add_input_list(S4_and_3, { i3_c, i2_c, i1_c, self.CI_not });

    local S4_nor_0 = Gate.NOR();
    Gate.add_input_list(S4_nor_0, { i3_s, S4_and_1, S4_and_2, S4_and_3 });

    self.Sum4_xor.add_input(S4_and_0);
    self.Sum4_xor.add_input(S4_nor_0);

    -- ====================================================
    -- Sum-3 logic
    -- ====================================================

    local S3_and_0 = Gate.AND();
    S3_and_0.add_input(i3_c);
    S3_and_0.add_inv_input(i3_s);

    local S3_and_1 = Gate.AND();
    local S3_and_2 = Gate.AND();

    Gate.add_input_list(S3_and_1, { i1_s, i2_c });
    Gate.add_input_list(S3_and_2, { i2_c, i1_c, self.CI_not });

    local S3_nor_0 = Gate.NOR();
    Gate.add_input_list(S3_nor_0, { i2_s, S3_and_1, S3_and_2 });

    self.Sum3_xor.add_input(S3_and_0);
    self.Sum3_xor.add_input(S3_nor_0);

    -- ====================================================
    -- Sum-2 logic
    -- ====================================================

    local S2_and_0 = Gate.AND();
    S2_and_0.add_input(i2_c);
    S2_and_0.add_inv_input(i2_s);

    local S2_and_1 = Gate.AND();
    Gate.add_input_list(S2_and_1, { i1_c, self.CI_not });

    local S2_nor_0 = Gate.NOR();
    Gate.add_input_list(S2_nor_0, { i1_s, S2_and_1 });

    self.Sum2_xor.add_input(S2_and_0);
    self.Sum2_xor.add_input(S2_nor_0);

    -- ====================================================
    -- Sum-2 logic
    -- ====================================================

    local S1_and = Gate.AND();
    local S1_not = Gate.NOR();

    S1_and.add_input(i1_c);
    S1_and.add_inv_input(i1_s);
    S1_not.add_input(self.CI_not);

    self.Sum1_xor.add_input(S1_and);
    self.Sum1_xor.add_input(S1_not);



    -- return fully constructed Adder
    return self;
end
