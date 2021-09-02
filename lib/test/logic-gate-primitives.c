#include <lib/test/logic-gate-primitives.h>

#ifdef __cplusplus
extern "C" {
#endif

#include <lib/logic.h>
#include <assert.h>

// test the six basic types
static void logic_gate_test_and(void);
static void logic_gate_test_nand(void);
static void logic_gate_test_or(void);
static void logic_gate_test_nor(void);
static void logic_gate_test_xor(void);
static void logic_gate_test_xnor(void);

static void logic_gate_test_flipflop(void);
static void logic_gate_test_flipflop_test_transition_to(
        logic_gate_t* ff, logic_gate_t* d, logic_gate_t* clk, int INPUT_VALUE, int OUTPUT_WANTED);


void logic_gate_test_primitives(void) {
    logic_gate_test_and();
    logic_gate_test_nand();
    logic_gate_test_or();
    logic_gate_test_nor();
    logic_gate_test_xor();
    logic_gate_test_xnor();

    logic_gate_test_flipflop();
}

static void logic_gate_test_flipflop_test_transition_to(
        logic_gate_t* ff, 
        logic_gate_t* d, 
        logic_gate_t* clk,
        int INPUT_VALUE,
        int OUTPUT_WANTED) {

    logic_gate_signal_set(clk, 0);
    logic_gate_signal_set(d, INPUT_VALUE);
    logic_gate_simulate();

    logic_gate_signal_set(clk, 1);
    logic_gate_simulate();
    assert(ff->output_value == OUTPUT_WANTED);
}

static void logic_gate_test_flipflop(void) {

    logic_init();

    logic_gate_t* ff  = FLIPFLOP();
    logic_gate_t* d   = SIGNAL();
    logic_gate_t* clk = SIGNAL();
    logic_gate_flipflop_set_data(ff, d);
    logic_gate_flipflop_set_clock(ff, clk);

    logic_gate_signal_set(d, 0);
    logic_gate_signal_set(clk, 0);
    logic_gate_simulate();
    assert(ff->output_value != OUTPUT_UND);

    const int Z = OUTPUT_HI_Z;
    const int U = OUTPUT_UND;    
    const int test_inputs[16][2] = {
        { U, U }, { 0, 0 }, { 0, 0 }, { U, U }, 
        { 0, 0 }, { 1, 1 }, { 1, 1 }, { 0, 0 }, 
        { 1, 1 }, { U, U }, { 1, 1 }, { Z, U },
        { 0, 0 }, { Z, U }, { U, U }, { 1, 1 }
    };

    int i;
    for(i = 0; i < 16; i++)
        logic_gate_test_flipflop_test_transition_to(
            ff, d, clk, test_inputs[i][0], test_inputs[i][1]);

    logic_clean();
    free(d);
    free(clk);
    assert(logic_allocator_ngates() == 0L);
}

static void logic_gate_test_and(void) {

    logic_init();

    logic_gate_t* g = AND();
    logic_gate_t* sig_a = SIGNAL();
    logic_gate_t* sig_b = SIGNAL();
    logic_gate_add_input(g, sig_a);
    logic_gate_add_input(g, sig_b);

    const int Z = OUTPUT_HI_Z;
    const int U = OUTPUT_UND;
    const int test_inputs[12][2+1] = {
        { 0, 0, 0 }, { 0, 1, 0 }, { 1, 0, 0 }, { 1, 1, 1 },
        { Z, 0, U }, { 0, Z, U }, { U, 0, U }, { 0, U, U },
        { Z, 1, U }, { 1, Z, U }, { U, 1, U }, { 1, U, U },
    };

    int i;
    for(i = 0; i < 12; i++) {
        logic_gate_signal_set(sig_a, test_inputs[i][0]);
        logic_gate_signal_set(sig_b, test_inputs[i][1]);
        logic_gate_simulate();
        assert(g->output_value == test_inputs[i][2]);
    }

    logic_clean();
    free(sig_a);
    free(sig_b);
    assert(logic_allocator_ngates() == 0L);
}

static void logic_gate_test_nand(void) {

    logic_init();

    logic_gate_t* g = NAND();
    logic_gate_t* sig_a = SIGNAL();
    logic_gate_t* sig_b = SIGNAL();
    logic_gate_add_input(g, sig_a);
    logic_gate_add_input(g, sig_b);

    const int Z = OUTPUT_HI_Z;
    const int U = OUTPUT_UND;
    const int test_inputs[12][2+1] = {
        { 0, 0, 1 }, { 0, 1, 1 }, { 1, 0, 1 }, { 1, 1, 0 },
        { Z, 0, U }, { 0, Z, U }, { U, 0, U }, { 0, U, U },
        { Z, 1, U }, { 1, Z, U }, { U, 1, U }, { 1, U, U },
    };

    int i;
    for(i = 0; i < 12; i++) {
        logic_gate_signal_set(sig_a, test_inputs[i][0]);
        logic_gate_signal_set(sig_b, test_inputs[i][1]);
        logic_gate_simulate();
        assert(g->output_value == test_inputs[i][2]);
    }

    logic_clean();
    free(sig_a);
    free(sig_b);
    assert(logic_allocator_ngates() == 0L);
}

static void logic_gate_test_or(void) {

    logic_init();

    logic_gate_t* g = OR();
    logic_gate_t* sig_a = SIGNAL();
    logic_gate_t* sig_b = SIGNAL();
    logic_gate_add_input(g, sig_a);
    logic_gate_add_input(g, sig_b);

    const int Z = OUTPUT_HI_Z;
    const int U = OUTPUT_UND;
    const int test_inputs[12][2+1] = {
        { 0, 0, 0 }, { 0, 1, 1 }, { 1, 0, 1 }, { 1, 1, 1 },
        { Z, 0, U }, { 0, Z, U }, { U, 0, U }, { 0, U, U },
        { Z, 1, U }, { 1, Z, U }, { U, 1, U }, { 1, U, U },
    };

    int i;
    for(i = 0; i < 12; i++) {
        logic_gate_signal_set(sig_a, test_inputs[i][0]);
        logic_gate_signal_set(sig_b, test_inputs[i][1]);
        logic_gate_simulate();
        assert(g->output_value == test_inputs[i][2]);
    }

    logic_clean();
    free(sig_a);
    free(sig_b);
    assert(logic_allocator_ngates() == 0L);
}

static void logic_gate_test_nor(void) {

    logic_init();

    logic_gate_t* g = NOR();
    logic_gate_t* sig_a = SIGNAL();
    logic_gate_t* sig_b = SIGNAL();
    logic_gate_add_input(g, sig_a);
    logic_gate_add_input(g, sig_b);

    const int Z = OUTPUT_HI_Z;
    const int U = OUTPUT_UND;
    const int test_inputs[12][2+1] = {
        { 0, 0, 1 }, { 0, 1, 0 }, { 1, 0, 1 }, { 1, 1, 0 },
        { Z, 0, U }, { 0, Z, U }, { U, 0, U }, { 0, U, U },
        { Z, 1, U }, { 1, Z, U }, { U, 1, U }, { 1, U, U },
    };

    int i;
    for(i = 0; i < 12; i++) {
        logic_gate_signal_set(sig_a, test_inputs[i][0]);
        logic_gate_signal_set(sig_b, test_inputs[i][1]);
        logic_gate_simulate();
        assert(g->output_value == test_inputs[i][2]);
    }

    logic_clean();
    free(sig_a);
    free(sig_b);
    assert(logic_allocator_ngates() == 0L);
}

static void logic_gate_test_xor(void) {

    logic_init();

    logic_gate_t* g = XOR();
    logic_gate_t* sig_a = SIGNAL();
    logic_gate_t* sig_b = SIGNAL();
    logic_gate_add_input(g, sig_a);
    logic_gate_add_input(g, sig_b);

    const int Z = OUTPUT_HI_Z;
    const int U = OUTPUT_UND;
    const int test_inputs[12][2+1] = {
        { 0, 0, 0 }, { 0, 1, 1 }, { 1, 0, 1 }, { 1, 1, 0 },
        { Z, 0, U }, { 0, Z, U }, { U, 0, U }, { 0, U, U },
        { Z, 1, U }, { 1, Z, U }, { U, 1, U }, { 1, U, U },
    };

    int i;
    for(i = 0; i < 12; i++) {
        logic_gate_signal_set(sig_a, test_inputs[i][0]);
        logic_gate_signal_set(sig_b, test_inputs[i][1]);
        logic_gate_simulate();
        assert(g->output_value == test_inputs[i][2]);
    }

    logic_clean();
    free(sig_a);
    free(sig_b);
    assert(logic_allocator_ngates() == 0L);
}

static void logic_gate_test_xnor(void) {

    logic_init();

    logic_gate_t* g = XNOR();
    logic_gate_t* sig_a = SIGNAL();
    logic_gate_t* sig_b = SIGNAL();
    logic_gate_add_input(g, sig_a);
    logic_gate_add_input(g, sig_b);

    const int Z = OUTPUT_HI_Z;
    const int U = OUTPUT_UND;
    const int test_inputs[12][2+1] = {
        { 0, 0, 1 }, { 0, 1, 0 }, { 1, 0, 0 }, { 1, 1, 1 },
        { Z, 0, U }, { 0, Z, U }, { U, 0, U }, { 0, U, U },
        { Z, 1, U }, { 1, Z, U }, { U, 1, U }, { 1, U, U },
    };

    int i;
    for(i = 0; i < 12; i++) {
        logic_gate_signal_set(sig_a, test_inputs[i][0]);
        logic_gate_signal_set(sig_b, test_inputs[i][1]);
        logic_gate_simulate();
        assert(g->output_value == test_inputs[i][2]);
    }

    logic_clean();
    free(sig_a);
    free(sig_b);
    assert(logic_allocator_ngates() == 0L);
}



#ifdef __cplusplus
} // extern C
#endif
