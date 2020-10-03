#pragma once

//
// logic_vector_t is used exlusively as an input wrapper
//

#include "logic-gates.h"

typedef struct logic_vector_t {
    logic_gate_t** gate_ptr;
    int size;
    logic_type type;
} logic_vector_t;

// heap allocate and initialize
logic_vector_t* logic_vector_new(int sz, logic_type type);

// initialize pre-allocated logic_vector structure
// returns pointer to allocated vector
logic_vector_t* logic_vector_init(logic_vector_t* vec, int sz, logic_type type);

// get logic gate ptr at specific index. index is checked internally
logic_gate_t* logic_vector_get_bit_at(logic_vector_t* vec, int idx);

// set logic gate ptr at specific index. index is checked internally
void logic_vector_set_bit_at(logic_vector_t* vec, int index, int value);

// set signals from string. this requires a string of the same length
void logic_vector_set_string(logic_vector_t* vec, const char* bitvalues);

// sets a grouping of bits in the vector. index is used to know where 
// to start modifying the vector from. bit vector does not have to 
// reach end of logic_vector. however, bitstrings that extend beyond 
// the vector size is an error
void logic_vector_set_string_idx(logic_vector_t* vec, const char* bitvalues, int start_idx);
