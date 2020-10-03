#pragma once

//
// logic_vector_t is used exlusively as an input wrapper
//

#include "logic-gates.h"

typedef struct logic_vector_t {
    logic_gate_t** gate_ptr;
    int size;
} logic_vector_t;

// heap allocate and initialize
logic_vector_t* logic_vector_new(int sz);

// initialize pre-allocated logic_vector structure
logic_vector_t* logic_vector_init(logic_vector_t* vec, int sz);

// get logic gate ptr at specific index. index is checked internally
logic_gate_t* logic_vector_get_bit_at(logic_vector_t* vec, int idx);

// set signals from string. this requires a string of the same length
// equivalent to calling:
// logic_vector_set_string_from(vec, bitvalues, 0) w/ some error checking
void logic_vector_set_string(logic_vector_t* vec, const char* bitvalues);

// sets a grouping of bits in the vector. index is used to know where 
// to start modifying the vector from. bit vector does not have to 
// reach end of logic_vector
void logic_vector_set_string_from(logic_vector_t* vec, const char* bitvalues, int start_idx);
