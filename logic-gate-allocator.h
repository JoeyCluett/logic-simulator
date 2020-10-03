#ifndef LOGIC_GATE_ALLOCATOR
#define LOGIC_GATE_ALLOCATOR

#ifdef __cplusplus
extern "C" {
#endif

#include "logic-gates.h"

// gates and inputs are allocated in chunks then simply doled 
// out as needed, allocating additional chunks as needed

// for testing purposes
//#define LOGIC_GATE_ALLOCATOR_CHUNK_SIZE  1
//#define LOGIC_INPUT_ALLOCATOR_CHUNK_SIZE 1

// try to allocate chunks of ~1KB
#define LOGIC_GATE_ALLOCATOR_CHUNK_SIZE  (1024 - 16) / sizeof(logic_gate_t)
#define LOGIC_INPUT_ALLOCATOR_CHUNK_SIZE (1024 - 16) / sizeof(logic_input_t)

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

    logic_input_chunk_t* input_first;
    logic_input_chunk_t* input_last;

} logic_allocator_t;

// perform logic gate allocator initialization
void logic_init(void);

// return the number of gates that have been allocated for simulation. 
// this does not include allocated input signal types 'SIGNAL()' that are used by toplevel entities
long int logic_allocator_ngates(void);

// return pointer to allocated logic_gate. no gate type information needed here
logic_gate_t* logic_gate_alloc(void);

// same deal as logic_gate_alloc but with inputs
logic_input_t* logic_input_alloc(void);

#ifdef __cplusplus
}
#endif


#endif // LOGIC_GATE_ALLOCATOR
