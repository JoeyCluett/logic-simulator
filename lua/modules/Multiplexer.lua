
require("simload");
require("gate");

Multiplexer = {};

Multiplexer.new = function(bitwidth)

    print('Multiplexer.new');

    local self = {};

    self.bitwidth = bitwidth;

    -- ==========================================
    -- set internal logic
    -- ==========================================

    self.inputs = {};
    self.inverted_inputs = {}

    for i=0,(bitwidth-1) do
        -- non-inverted inputs

        print('Multiplexer.new : allocating OR...');

        self.inputs[i] = Gate.OR();

        print('Multiplexer.new : allocating NOR...');

        -- inverted inputs
        self.inverted_inputs[i] = Gate.NOR();
        --self.inverted_inputs[i].add_input(self.inputs[i]);
    end

    self.output_or = Gate.OR();
    self.outputs = {};

    local n_outputs = math.pow(2, bitwidth);

    print('Multiplexer.new : num output bits: ' .. (n_outputs >> 0));

    for i=0,(n_outputs - 1) do
        local gate_and = Gate.AND();

        local val = i;

        for j=0,(bitwidth-1) do
            local t = (val >> j) & 0x01            

            if     t == 0 then gate_and.add_input(self.inverted_inputs[j]);
            elseif t == 1 then gate_and.add_input(self.inputs[j]);
            else error('Multiplexer : bit value is not 0 or 1');
            end

        end

        self.outputs[i] = gate_and;
        self.output_or.add_input(gate_and);
    end

    -- ==========================================
    -- interface methods
    -- ==========================================

    self.set_Input = function(index, gate)
        if index >= math.pow(2, self.bitwidth) or index < 0 then
            error('Multiplexer.set_Input : index \'' .. index .. '\' is out or range');
        else
            self.outputs[index].add_input(gate);
        end
    end

    self.set_Select = function(index, gate)
        if index >= self.bitwidth or index < 0 then
            error('Multiplexer.set_Select : index \'' .. index .. '\' is out of range');
        else
            self.inputs[index].add_input(gate);
            self.inverted_inputs[index].add_input(gate);
        end
    end

    self.get_Output = function()
        return self.output_or;
    end

    return self;
end
