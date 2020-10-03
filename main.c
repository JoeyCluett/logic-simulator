#include <stdio.h>
#include <assert.h>

#include "logic-gates.h"
#include "logic-gate-allocator.h"
#include "logic-gate-utility.h"

#include "modules/adders.h"
#include "modules/nbit-register.h"

#define BITWIDTH 4

full_adder_t* fa;

void callback(void) {}

int main(int argc, char** argv) {

    //printf("sizeof logic_gate_t        : %lu bytes\n", sizeof(logic_gate_t));
    //printf("sizeof logic_gate_chunk_t  : %lu bytes\n", sizeof(logic_gate_chunk_t));
    //printf("sizeof logic_input_chunk_t : %lu bytes\n", sizeof(logic_input_chunk_t));

    logic_init(); // initialize all logic subsystems

    logic_gate_t* Inputs[3];
    logic_gate_t* Outputs[3];
    logic_gate_t* Clock;

    nbit_register_t* reg = nbit_register_new(3);

    Clock = SIGNAL();

    // initialize inputs and outputs
    int i;
    for(i = 0; i < 3; i++) {
        Inputs[i] = SIGNAL();
        nbit_register_set_data(reg, Inputs[i], i);
        Outputs[i] = nbit_register_get_q(reg, i);
    }

    nbit_register_set_clock(reg, Clock);

    printf("Clk  b0  b1  b2\n");

    for(i = 0; i < 8; i++) {

        logic_gate_signal_set(Inputs[0], (i >> 0) & 0x1);
        logic_gate_signal_set(Inputs[1], (i >> 1) & 0x1);
        logic_gate_signal_set(Inputs[2], (i >> 2) & 0x1);

        logic_gate_signal_set(Clock, 0);
        logic_gate_simulate();
        
        printf("%d    %d  %d  %d\n", 
                Clock->output_value, 
                reg->reg_ptr[0]->output_value,
                reg->reg_ptr[1]->output_value,
                reg->reg_ptr[2]->output_value);
        
        logic_gate_signal_set(Clock, 1);
        logic_gate_simulate();

        printf("%d    %d  %d  %d\n", 
                Clock->output_value, 
                reg->reg_ptr[0]->output_value,
                reg->reg_ptr[1]->output_value,
                reg->reg_ptr[2]->output_value);

    }

/*
    rca_adder_t* adder = rca_adder_new(BITWIDTH);
    logic_gate_t* A[BITWIDTH];
    logic_gate_t* B[BITWIDTH];
    logic_gate_t* Cin = SIGNAL();

    {
        int i;
        for(i = 0; i < BITWIDTH; i++) {
            A[i] = SIGNAL();
            B[i] = SIGNAL();

            rca_adder_set_a(adder, A[i], i);
            rca_adder_set_b(adder, B[i], i);
        }
    }

    rca_adder_set_ci(adder, Cin);

    // cin will always be zero
    logic_gate_signal_set(Cin, 0);

    int i, j;
    for(i = 0; i < 4; i++) {
        for(j = 0; j < 4; j++) {

            logic_gate_signal_set(A[0], !!(i & 0x01));
            logic_gate_signal_set(A[1], !!(i & 0x02));
            logic_gate_signal_set(A[2], !!(i & 0x04));
            logic_gate_signal_set(A[3], !!(i & 0x08));

            logic_gate_signal_set(B[0], !!(j & 0x01));
            logic_gate_signal_set(B[1], !!(j & 0x02));
            logic_gate_signal_set(B[2], !!(j & 0x04));
            logic_gate_signal_set(B[3], !!(j & 0x08));

            logic_gate_simulate();

            printf(" %d%d%d%d\n", A[3]->output_value, A[2]->output_value, A[1]->output_value, A[0]->output_value);
            printf(" %d%d%d%d\n", B[3]->output_value, B[2]->output_value, B[1]->output_value, B[0]->output_value);
            printf("%d%d%d%d%d\n\n\n", 
                    rca_adder_get_carry(adder)->output_value,
                    rca_adder_get_sum(adder, 3)->output_value,
                    rca_adder_get_sum(adder, 2)->output_value,
                    rca_adder_get_sum(adder, 1)->output_value,
                    rca_adder_get_sum(adder, 0)->output_value);

            int tmp = 0;
            int k = 0;
            for(; k < 4; k++)
                tmp |= (rca_adder_get_sum(adder, k)->output_value << k);
            tmp |= (rca_adder_get_carry(adder)->output_value << 4);

            // ensure correct results are coming out the other end
            assert((i + j) == tmp);
        }
    }
*/

/*
    // a few logic signals to play with
    logic_gate_t* A  = SIGNAL();
    logic_gate_t* B  = SIGNAL();
    logic_gate_t* Ci = SIGNAL();

    //full_adder_t* fa = full_adder_new();
    fa = full_adder_new();

    full_adder_set_a(fa, A);
    full_adder_set_b(fa, B);
    full_adder_set_ci(fa, Ci);

    printf("num sim-allocated gates : %ld\n\n\n", logic_allocator_ngates());


    int i = 0;
    for(; i < 8; i++) {
        logic_gate_signal_set(A,  !!(i & 0x01));
        logic_gate_signal_set(B,  !!(i & 0x02));
        logic_gate_signal_set(Ci, !!(i & 0x04));

        puts("\n\n");
        printf("A B Ci\n%d %d %d\n", A->output_value, B->output_value, Ci->output_value);

        // perform simulation run
        logic_gate_simulate();
        //logic_gate_simulate_callback(callback);

        printf("lower and n high : %d\n", fa->lower->carry->output_value);
        printf("upper and n high : %d\n\n", fa->upper->carry->output_value);

        printf("lower xor : %d\n", fa->lower->sum->output_value);
        printf("lower and : %d\n", fa->lower->carry->output_value);
        printf("upper xor : %d\n", fa->upper->sum->output_value);
        printf("upper and : %d\n", fa->upper->sum->output_value);
        printf("carry out : %d\n\n", fa->carry_out->output_value);

        printf("Sum Co\n%d   %d\n", fa->upper->sum->output_value, fa->carry_out->output_value);

    }
*/
    return 0;
}
