#pragma once

#include <stdlib.h>
#include <array>

//
// because STL implementation does not use bits as underlying type
// maintainers abandoned that idea after the std::vector fiasco
//

template<size_t sz>
struct BoolArray_t {

    std::array<unsigned char, (sz/8)+1> arr;

    const size_t arr_len = sz;

    BoolArray_t(bool start_value);

    void set(size_t index, bool value);

    bool get(size_t index);
};

template<size_t sz>
BoolArray_t<sz>::BoolArray_t(bool start_value) {
    if(start_value) {
        for(size_t i = 0UL; i < this->arr.size(); i++) {
            this->arr[i] = 0xFF;
        }
    }
    else {
        for(size_t i = 0UL; i < this->arr.size(); i++) {
            this->arr[i] = 0x00;
        }
    }
}

template<size_t sz>
void BoolArray_t<sz>::set(size_t index, bool value) {

    const int bit_idx = index & 0b111;
    const size_t byte_index = index >> 3;

    if(value) {
        const unsigned char or_mask  = 0x1 << bit_idx;
        this->arr.at(byte_index) |= or_mask;
    }
    else {
        const unsigned char and_mask = ~(0x1 << bit_idx);
        this->arr.at(byte_index) &= and_mask;
    }
}

template<size_t sz>
bool BoolArray_t<sz>::get(size_t index) {
    const size_t byte_index = index >> 3;
    const int bit_index = index & 0b111;
    const unsigned char byte = this->arr.at(byte_index);
    return ((byte >> bit_index) & 0x01) ? true : false;
}

