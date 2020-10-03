#include <stdio.h>
#include <string.h>

#include "logic-gates.h"
#include "logic-vector.h"
#include "logic-gate-utility.h"

static void logic_vector_verify_index(logic_vector_t* vec, int idx) {
    if(idx < 0 || idx >= vec->size) {
        fprintf(stderr, "logic_vector_verify_index : index '%d' is out of range\n", idx);
        exit(1);
    }
}

logic_vector_t* logic_vector_new(int sz, logic_type type) {
    logic_vector_t* vec = (logic_vector_t*)malloc(sizeof(logic_vector_t));
    return logic_vector_init(vec, sz, type);
}

logic_vector_t* logic_vector_init(logic_vector_t* vec, int sz, logic_type type) {
    vec->size     = sz;
    vec->type     = type;
    vec->gate_ptr = (logic_gate_t**)malloc(sizeof(logic_gate_t*) * sz);

    int i;
    for(i = 0; i < sz; i++)
        vec->gate_ptr[i] = SIGNAL();

    return vec;
}

logic_gate_t* logic_vector_get_bit_at(logic_vector_t* vec, int idx) {
    logic_vector_verify_index(vec, idx);
    return vec->gate_ptr[idx];
}

void logic_vector_set_string(logic_vector_t* vec, const char* bitvalues) {
    
    int slen = strlen(bitvalues);

    if(slen != vec->size) {
        fprintf(stderr, "logic_vector_set_string : bit string '%s' is longer than logic vector (size=%d)\n", bitvalues, slen);
        exit(1);
    }

    char* iter = (char*)bitvalues;
    int idx = 0;

    while(*iter) {

        if(*iter == '0') {      vec->gate_ptr[idx]->output_value = 0; }
        else if(*iter == '1') { vec->gate_ptr[idx]->output_value = 1; }
        else {
            fprintf(stderr, "logic_vector_set_string : invalid character '%c' found in bit string '%s'", *iter, bitvalues);
            exit(1);
        }

        iter++; // advance to next character
        idx++;  // advance to next index
    }

}
