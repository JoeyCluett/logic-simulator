#include "logic-gate-allocator.h"

#ifdef __cplusplus
extern "C" {
#endif

logic_allocator_t global_logic_allocator;
extern logic_gate_t* ONE;
extern logic_gate_t* ZERO;

long int logic_allocator_ngates(void) {
    long int ngates = 0;

    logic_gate_chunk_t* chunk = global_logic_allocator.gate_first;
    while(chunk != NULL) {
        ngates += chunk->n_allocated;
        chunk = chunk->next;
    }

    return ngates;
}

static void logic_init_gate_allocator(void) {
    fprintf(stderr, "initializing gate allocator...\n");

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

static void logic_init_input_allocator(void) {
    fprintf(stderr, "initializing input allocator...\n");

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
    fprintf(stderr, "pre-allocating additional gate chunk...\n");

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

static void logic_allocator_realloc_inputs(void) {
    fprintf(stderr, "pre-allocating additional input chunk...\n");

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
    logic_init_input_allocator();

    // heap allocate these globals
    ONE = logic_gate_new(logic_gate_constant);
    ONE->output_value = 1;

    ZERO = logic_gate_new(logic_gate_constant);
    ZERO->output_value = 0;
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

