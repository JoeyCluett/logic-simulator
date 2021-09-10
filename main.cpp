#include <iostream>

#include <lib/logic.h>
#include <lib/exit_hook.h>
//#include <lib/test/logic-gate-primitives.h>

#include <lib/modules/adders/CarryLookaheadAdder.h>
#include <lib/modules/Decoder.h>
#include <lib/modules/Register.h>
#include <lib/modules/homebrew/RegisterFile.h>
#include <lib/modules/homebrew/HomebrewCPU.h>

#include <lib/modules/riscv/rv32i-decoder.h>

#include <lib/generate/Netlist.h>

#include <lib/cpp-api/gate-wrapper.h>
#include <lib/cpp-api/signal-wrapper.h>
#include <lib/cpp-api/throw-exception.h>

#include <lib/modules/riscv/rv32i-cpu.h>

using namespace std;

int main(int argc, char** argv) {

    //RISCV_Decoder_t::Test();

    logic_init();

    RISCV_Cpu_t cpu;

    logic_allocator_debug_info();
    logic_clean();

/*
    {
        logic_init();

        RISCV_Decoder_t decoder;

        SignalVector_t sig(32);
        Signal_t       clk;

        VectorConnect_(decoder.ir.register_, sig);
        decoder.ir.clk_input.add_input(clk);

        generate_netlist_file(
            "txt/riscv-decoder.txt",
            "riscv_decoder",
            {},
            {
                { "Instruction", sig }
            },
            {},
            {
                { "DecodedBit", VectorCvt_(decoder.instruction_bit_value) }
            }
        );

        logic_allocator_debug_info();
        logic_clean();
    }
*/

    return 0;
}
