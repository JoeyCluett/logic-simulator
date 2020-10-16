
require("simload");
require("gate");

Selector_NBit = {};
Selector_NBit.new = function(bitwidth)

    local self = {};

    self.bitwidth = bitwidth;

    self.inputs_0 = {};
    self.inputs_1 = {};
    self.outputs  = {};

    self.not_gate = Gate.NOR();

    for i=0,(bitwidth-1) do
        self.inputs_0[i] = Gate.AND(self.not_gate);
        self.inputs_1[i] = Gate.AND();
        self.outputs[i]  = Gate.OR( self.inputs_0[i], self.inputs_1[i] );
    end

    -- ==========================================
    -- interface methods
    -- ==========================================

    self.set_Select = function(gate)
        self.not_gate.add_input(gate);
        for i=0,(self.bitwidth - 1) do self.inputs_1[i].add_input(gate); end
    end

    self.set_Input0 = function(index, gate)
        if index >= (self.bitwidth - 1) or index < 0 then
            error('Selector_NBit.set_Input0 : index \'' .. index .. '\' is out of range');
        else
            self.inputs_0[index].add_input(gate);
        end
    end

    self.set_Input1 = function(index, gate)
        if index >= (self.bitwidth - 1) or index < 0 then
            error('Selector_NBit.set_Input1 : index \'' .. index .. '\' is out of range');
        else
            self.inputs_1[index].add_input(gate);
        end
    end

    self.get_Output = function(index)
        if index >= (self.bitwidth - 1) or index < 0 then
            error('Selector_NBit.get_Output : index \'' .. index .. '\' is out of range');
        else
            return self.outputs[index];
        end
    end

    return self;
end