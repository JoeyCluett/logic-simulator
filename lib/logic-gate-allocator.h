#ifndef LOGIC_GATE_ALLOCATOR
#define LOGIC_GATE_ALLOCATOR

#include "logic-gates.h"

#ifdef __cplusplus
extern "C" {
#endif

//
// gates and inputs are allocated in chunks then simply doled 
// out as needed, allocating additional chunks as needed
// - allocator is very naive
//      - assumes gates are never free'd individually
//      - logic_clean() frees all allocated gates
// - chunk size can be changed to any size (for testing purposes)
// - gate entities AND inputs are allocated here
//      - results in a netlist that is less likely to be scattered around memory
// - for simulation purposes, a flip-flop is considered a primitive gate type and 
//   its rising-edge behavior is natively handled in the simulation loop
// - only makes occasional system calls to allocate more memory
//

// for testing purposes
//#define LOGIC_GATE_ALLOCATOR_CHUNK_SIZE  3
//#define LOGIC_INPUT_ALLOCATOR_CHUNK_SIZE 3

// try to allocate chunks of ~64KB
#define LOGIC_GATE_ALLOCATOR_CHUNK_SIZE  ((64 * 1024) - 16) / sizeof(logic_gate_t)
#define LOGIC_INPUT_ALLOCATOR_CHUNK_SIZE ((64 * 1024) - 16) / sizeof(logic_input_t)

typedef struct logic_gate_chunk_t {
    struct logic_gate_chunk_t* next; // maintain list of malloc()'ed chunks
    long int n_allocated; // used to determine if we need another chunk
    logic_gate_t gate_chunk[LOGIC_GATE_ALLOCATOR_CHUNK_SIZE];
} logic_gate_chunk_t;

typedef struct logic_input_chunk_t {
    struct logic_input_chunk_t* next;
    long int n_allocated;
    logic_input_t input_chunk[LOGIC_INPUT_ALLOCATOR_CHUNK_SIZE];
} logic_input_chunk_t;

// maintains two linked list of malloc()'ed data. 
// this allows easier cleanup after the program is done
typedef struct logic_allocator_t {
    logic_gate_chunk_t*  gate_first;
    logic_gate_chunk_t*  gate_last;

    // forward types are allocated seperately 
    // from other functional gate types
    logic_gate_chunk_t*  forward_first;
    logic_gate_chunk_t*  forward_last;

    logic_input_chunk_t* input_first;
    logic_input_chunk_t* input_last;

} logic_allocator_t;

// perform logic gate allocator initialization
void logic_init(void);

// delete all allocated logic gates
void logic_clean(void);

// return the number of gates that have been allocated for simulation. 
// this does not include allocated input signal types 'SIGNAL()' that are used by toplevel entities
long int logic_allocator_ngates(void);

// return pointer to allocated logic_gate. no gate type information needed here
logic_gate_t* logic_gate_alloc(void);

// same deal as logic_gate_alloc but with inputs
logic_input_t* logic_input_alloc(void);

// print debug allocator info
void logic_allocator_debug_info(void);

// return address of logic allocator
logic_allocator_t* logic_allocator_get_reference(void);

logic_gate_t* logic_gate_alloc_forward(void);

// simulation callback support
typedef void(*gate_simulate_callback)(void);
void logic_gate_simulate_set_precallback(gate_simulate_callback cb);
void logic_gate_simulate_set_postcallback(gate_simulate_callback cb);

#ifdef __cplusplus
} // extern "C"
#endif

#endif // LOGIC_GATE_ALLOCATOR
