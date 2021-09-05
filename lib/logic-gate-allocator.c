#include <lib/logic-gate-allocator.h>

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#ifdef __cplusplus
extern "C" {
#endif

logic_allocator_t global_logic_allocator;
extern logic_gate_t* ONE;
extern logic_gate_t* ZERO;

logic_allocator_t* logic_allocator_get_reference(void) {
    return &global_logic_allocator;
}

long int logic_allocator_ngates(void) {
    long int ngates = 0;

    logic_gate_chunk_t* chunk = global_logic_allocator.gate_first;
    while(chunk != NULL) {
        ngates += chunk->n_allocated;
        chunk = chunk->next;
    }

    return ngates;
}

void logic_allocator_debug_info(void) {

    long int n_allocd_gates = logic_allocator_ngates();
    unsigned long n_real_gates = 0UL;
    unsigned long n_forward_gates = 0UL;
    unsigned long n_allocd_inputs = 0UL;
    int gate_chunks_allocd_real  = 0;
    int gate_chunks_allocd_fwd   = 0;
    int input_chunks_allocd = 0;

    logic_gate_chunk_t* gate_chunk_ptr = global_logic_allocator.gate_first;
    while(gate_chunk_ptr != NULL) {
        gate_chunks_allocd_real++;

        logic_gate_t* gate_iter = gate_chunk_ptr->gate_chunk;
        logic_gate_t* gate_iter_end = gate_chunk_ptr->gate_chunk + gate_chunk_ptr->n_allocated;

        while(gate_iter != gate_iter_end) {

            switch(gate_iter->gate_type) {
                case logic_gate_and: case logic_gate_nand:
                case logic_gate_or: case logic_gate_nor:
                case logic_gate_xor: case logic_gate_xnor:
                case logic_gate_flipflop:
                //case logic_gate_constant:
                    n_real_gates++;
                    break;
            
                case logic_gate_signal:
                    break;
                case logic_gate_forward:
                    n_forward_gates++;
                    break;
                case logic_gate_buffer:
                case logic_gate_wire:
                    n_real_gates++;
                default:
                    break;
            }

            gate_iter++;
        }

        gate_chunk_ptr = gate_chunk_ptr->next;
    }

    //assert(n_forward_gates == 0ul);

    gate_chunk_ptr = global_logic_allocator.forward_first;
    while(gate_chunk_ptr != NULL) {
        gate_chunks_allocd_fwd++;
        n_forward_gates += gate_chunk_ptr->n_allocated;
        gate_chunk_ptr = gate_chunk_ptr->next;
    }

    logic_input_chunk_t* input_chunk_ptr = global_logic_allocator.input_first;
    while(input_chunk_ptr != NULL) {
        input_chunks_allocd++;
        n_allocd_inputs += input_chunk_ptr->n_allocated;
        input_chunk_ptr = input_chunk_ptr->next;
    }

    printf("\n  allocator debug info:\n");
    printf("      allocated gates:       %ld\n", n_allocd_gates+n_forward_gates);
    printf("          real:              %lu\n", n_real_gates);
    printf("          forward:           %lu\n", n_forward_gates);
    printf("          constants:         2\n");
    printf("      allocated inputs:      %lu\n", n_allocd_inputs);
    printf("      allocated gate chunks: %d\n", gate_chunks_allocd_real + gate_chunks_allocd_fwd);
    printf("          real:              %d\n", gate_chunks_allocd_real);
    printf("          forward:           %d\n", gate_chunks_allocd_fwd);
    printf("      allocated input chunk: %d\n", input_chunks_allocd);
    printf("      gate chunk size:       %lu gates (%lu bytes)\n", 
            (unsigned long)LOGIC_GATE_ALLOCATOR_CHUNK_SIZE, 
            (unsigned long)sizeof(logic_gate_chunk_t));
    printf("      input chunk size:      %lu inputs (%lu bytes)\n\n", 
            (unsigned long)LOGIC_INPUT_ALLOCATOR_CHUNK_SIZE, 
            (unsigned long)sizeof(logic_input_chunk_t));
}

static void logic_init_gate_allocator(void) {
    //fprintf(stderr, "    initializing gate allocator...\n");

    logic_gate_chunk_t* gchunk = (logic_gate_chunk_t*)malloc(sizeof(logic_gate_chunk_t));

    // setup our two iterators
    logic_gate_t* begin_ptr = gchunk->gate_chunk;
    logic_gate_t* end_ptr   = begin_ptr + LOGIC_GATE_ALLOCATOR_CHUNK_SIZE;

    // all gates need to be initialized to none type first
    while(begin_ptr != end_ptr) {
        begin_ptr->gate_type    = logic_gate_none;
        begin_ptr->output_value = 0;
        ++begin_ptr;
    }

    gchunk->n_allocated = 0;
    gchunk->next = NULL; // last pointer in the sequence will always be NULL
                         // this is how we iterate through them

    global_logic_allocator.gate_first = gchunk;
    global_logic_allocator.gate_last  = gchunk;
}

static void logic_init_forward_allocator(void) {

    logic_gate_chunk_t* gate_chunk = (logic_gate_chunk_t*)malloc(sizeof(logic_gate_chunk_t));

    logic_gate_t* begin_ptr = gate_chunk->gate_chunk;
    const logic_gate_t* end_ptr = begin_ptr + LOGIC_GATE_ALLOCATOR_CHUNK_SIZE;

    while(begin_ptr != end_ptr) {
        begin_ptr->gate_type = logic_gate_none;
        begin_ptr->output_value = 0;
        ++begin_ptr;
    }

    gate_chunk->n_allocated = 0;
    gate_chunk->next = NULL;

    global_logic_allocator.forward_first = gate_chunk;
    global_logic_allocator.forward_last  = gate_chunk;

}

static void logic_init_input_allocator(void) {
    //fprintf(stderr, "    initializing input allocator...\n");

    logic_input_chunk_t* ichunk = (logic_input_chunk_t*)malloc(sizeof(logic_input_chunk_t));

    logic_input_t* begin_ptr = ichunk->input_chunk;
    logic_input_t* end_ptr   = begin_ptr + LOGIC_INPUT_ALLOCATOR_CHUNK_SIZE;

    while(begin_ptr != end_ptr) {
        begin_ptr->output_ptr = NULL;
        begin_ptr->next = NULL;
        ++begin_ptr;
    }

    ichunk->n_allocated = 0;
    ichunk->next        = NULL;

    global_logic_allocator.input_first = ichunk;
    global_logic_allocator.input_last  = ichunk;
}



static void logic_allocator_realloc_gates(void) {
    //fprintf(stderr, "    pre-allocating additional gate chunk...\n");

    logic_gate_chunk_t* newchunk = (logic_gate_chunk_t*)malloc(sizeof(logic_gate_chunk_t));

    // setup iterators
    logic_gate_t* begin_ptr = newchunk->gate_chunk;
    logic_gate_t* end_ptr   = begin_ptr + LOGIC_GATE_ALLOCATOR_CHUNK_SIZE;

    // iterate over and initialize newly allocated logic_gate_t structs
    while(begin_ptr != end_ptr) {
        begin_ptr->gate_type = logic_gate_none;
        begin_ptr->gate.inputs = NULL;
        ++begin_ptr;
    }

    newchunk->n_allocated = 0;
    newchunk->next = NULL;

    // new chunk are always placed at the end. this allows us 
    // to shortcut the simulation phase properly
    global_logic_allocator.gate_last->next = newchunk;
    global_logic_allocator.gate_last = newchunk;
}

static void logic_allocator_realloc_forward_gates(void) {
    //fprintf(stderr, "    pre-allocating additional forward gate chunk...\n");

    logic_gate_chunk_t* newchunk = (logic_gate_chunk_t*)malloc(sizeof(logic_gate_chunk_t));

    logic_gate_t* begin_ptr = newchunk->gate_chunk;
    const logic_gate_t* end_ptr   = begin_ptr + LOGIC_GATE_ALLOCATOR_CHUNK_SIZE;

    while(begin_ptr != end_ptr) {
        begin_ptr->gate_type = logic_gate_none;
        begin_ptr->gate.inputs = NULL;
        ++begin_ptr;
    }

    newchunk->n_allocated = 0;
    newchunk->next = NULL;

    global_logic_allocator.forward_last->next = newchunk;
    global_logic_allocator.forward_last = newchunk;
}

static void logic_allocator_realloc_inputs(void) {
    fprintf(stderr, "    pre-allocating additional input chunk...\n");

    logic_input_chunk_t* newchunk = (logic_input_chunk_t*)malloc(sizeof(logic_input_chunk_t));

    logic_input_t* begin_ptr = newchunk->input_chunk;
    logic_input_t* end_ptr = begin_ptr + LOGIC_INPUT_ALLOCATOR_CHUNK_SIZE;
    while(begin_ptr != end_ptr) {
        begin_ptr->output_ptr = NULL;
        begin_ptr->next       = NULL;
        ++begin_ptr;
    }

    newchunk->n_allocated = 0;
    newchunk->next = NULL;

    global_logic_allocator.input_last->next = newchunk;
    global_logic_allocator.input_last = newchunk;
}

void logic_init(void) {
    logic_init_gate_allocator();
    logic_init_forward_allocator();
    logic_init_input_allocator();

    // heap allocate these globals
    ONE = logic_gate_new(logic_gate_constant);
    ONE->output_value = 1;

    ZERO = logic_gate_new(logic_gate_constant);
    ZERO->output_value = 0;
}

void logic_clean(void) {

    logic_gate_chunk_t* gate_ptr = global_logic_allocator.gate_first;

    while(gate_ptr != NULL) {
        logic_gate_chunk_t* tmp_chunk_ptr = gate_ptr->next;
        free((void*)gate_ptr);
        gate_ptr = tmp_chunk_ptr;
    }

    gate_ptr = global_logic_allocator.forward_first;

    while(gate_ptr != NULL) {
        logic_gate_chunk_t* tmp_chunk_ptr = gate_ptr->next;
        free((void*)gate_ptr);
        gate_ptr = tmp_chunk_ptr;
    }

    logic_input_chunk_t* input_ptr = global_logic_allocator.input_first;

    while(input_ptr != NULL) {
        logic_input_chunk_t* tmp_chunk_ptr = input_ptr->next;
        free((void*)input_ptr);
        input_ptr = tmp_chunk_ptr;
    }

    global_logic_allocator.gate_first = NULL;
    global_logic_allocator.gate_last = NULL;

    global_logic_allocator.forward_first = NULL;
    global_logic_allocator.forward_last = NULL;

    global_logic_allocator.input_first = NULL;
    global_logic_allocator.input_last = NULL;

    free(ONE);
    free(ZERO);
}

logic_gate_t* logic_gate_alloc(void) {

    if(global_logic_allocator.gate_last->n_allocated == LOGIC_GATE_ALLOCATOR_CHUNK_SIZE)
        logic_allocator_realloc_gates();

    logic_gate_chunk_t* chunk_ptr = global_logic_allocator.gate_last;
    logic_gate_t* gate_ptr = chunk_ptr->gate_chunk + chunk_ptr->n_allocated;
                             // gotta love pointer math

    chunk_ptr->n_allocated++;
    return gate_ptr;
}

logic_gate_t* logic_gate_alloc_forward(void) {
    if(global_logic_allocator.forward_last->n_allocated == LOGIC_GATE_ALLOCATOR_CHUNK_SIZE)
        logic_allocator_realloc_forward_gates();

    logic_gate_chunk_t* chunk_ptr = global_logic_allocator.forward_last;
    logic_gate_t* gate_ptr = chunk_ptr->gate_chunk + chunk_ptr->n_allocated;

    chunk_ptr->n_allocated++;
    return gate_ptr;
}

logic_input_t* logic_input_alloc(void) {

    if(global_logic_allocator.input_last->n_allocated == LOGIC_INPUT_ALLOCATOR_CHUNK_SIZE)
        logic_allocator_realloc_inputs();

    logic_input_chunk_t* chunk_ptr = global_logic_allocator.input_last;
    logic_input_t* input_ptr = chunk_ptr->input_chunk + chunk_ptr->n_allocated;

    chunk_ptr->n_allocated++;
    return input_ptr;
}

#ifdef __cplusplus
}
#endif
