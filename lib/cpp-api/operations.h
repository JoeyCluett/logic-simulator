#pragma once

#include <lib/cpp-api/primitive-type-traits.h>

namespace Op {

//
// for testing purposes more than anything
//
template<typename T>
struct operation_result_t {
    union {
        bool carry;
        bool borrow;
    };

    T result;
};

struct intermediate_t {
    union {
        char i8; unsigned char u8;
        short i16; unsigned short u16;
        int i32; unsigned int u32;
        long int i64; unsigned long int u64;
        float f32; double f64;
    };
};

template<typename T>
operation_result_t<T> add(T _lhs, T _rhs) {

    operation_result_t<T> res;
    res.carry = 0;
    intermediate_t lhs;
    intermediate_t rhs;
    intermediate_t ans;

    if(is_i8<T>()) {
        lhs.i8 = _lhs; rhs.i8 = _rhs;
        ans.u8 = lhs.u8 + rhs.u8;
        if(ans.u8 < lhs.u8 && ans.u8 < rhs.u8)
            res.carry = 1;
        res.result = ans.i8;
    }
    else if(is_i16<T>()) {
        lhs.i16 = _lhs; rhs.i16 = _rhs;
        ans.u16 = lhs.u16 + rhs.u16;
        if(ans.u16 < lhs.u16 && ans.u16 < rhs.u16)
            res.carry = 1;
        res.result = ans.i16;
    }
    else if(is_i32<T>()) {
        lhs.i32 = _lhs; rhs.i32 = _rhs;
        ans.u32 = lhs.u32 + rhs.u32;
        if(ans.u32 < lhs.u32 && ans.u32 < rhs.u32)
            res.carry = 1;
        res.result = ans.i32;
    }
    else if(is_i64<T>()) {
        lhs.i64 = _lhs; rhs.i64 = _rhs;
        ans.u64 = lhs.u64 + rhs.u64;
        if(ans.u64 < lhs.u64 && ans.u64 < rhs.u64)
            res.carry = 1;
        res.result = ans.i64;
    }
    else if(is_u8<T>()) {
        lhs.u8 = _lhs; rhs.u8 = _rhs;
        ans.u8 = lhs.u8 + rhs.u8;
        if(ans.u8 < lhs.u8 && ans.u8 < rhs.u8)
            res.carry = 1;
        res.result = ans.u8;
    }
    else if(is_u16<T>()) {
        lhs.u16 = _lhs; rhs.u16 = _rhs;
        ans.u16 = lhs.u16 + rhs.u16;
        if(ans.u16 < lhs.u16 && ans.u16 < rhs.u16)
            res.carry = 1;
        res.result = ans.u16;
    }
    else if(is_u32<T>()) {
        lhs.u32 = _lhs; rhs.u32 = _rhs;
        ans.u32 = lhs.u16 + rhs.u16;
        if(ans.u32 < lhs.u32 && ans.u32 < rhs.u32)
            res.carry = 1;
        res.result = ans.u32;
    }
    else if(is_u64<T>()) {
        lhs.u64 = _lhs; rhs.u64 = _rhs;
        ans.u64 = lhs.u8 + rhs.u8;
        if(ans.u64 < lhs.u64 && ans.u64 < rhs.u64)
            res.carry = 1;
        res.result = ans.u64;
    }



    else if(is_f32<T>()) {
        res.result = _lhs + _rhs;
    }
    else if(is_f64<T>()) {
        res.result = _lhs + _rhs;
    }


    else {
        throw std::runtime_error(__function__);
    }


    return res;
}




template<typename T>
operation_result_t<T> subtract(T _lhs, T _rhs) {

    operation_result_t<T> res;
    res.carry = 0;
    intermediate_t lhs;
    intermediate_t rhs;
    intermediate_t ans;

    if(is_i8<T>()) {
        lhs.i8 = _lhs; rhs.i8 = _rhs;
        ans.u8 = lhs.u8 + ~rhs.u8;
        ans.u8++;
        if(ans.u8 < lhs.u8 && ans.u8 < rhs.u8)
            res.borrow = 1;
        res.result = ans.i8;
    }
    else if(is_i16<T>()) {
        lhs.i16 = _lhs; rhs.i16 = _rhs;
        ans.u16 = lhs.u16 + ~rhs.u16;
        ans.u16++;
        if(ans.u16 < lhs.u16 && ans.u16 < rhs.u16)
            res.borrow = 1;
        res.result = ans.i16;
    }
    else if(is_i32<T>()) {
        lhs.i32 = _lhs; rhs.i32 = _rhs;
        ans.u32 = lhs.u32 + ~rhs.u32;
        ans.u32++;
        if(ans.u32 < lhs.u32 && ans.u32 < rhs.u32)
            res.borrow = 1;
        res.result = ans.i32;
    }
    else if(is_i64<T>()) {
        lhs.i64 = _lhs; rhs.i64 = _rhs;
        ans.u64 = lhs.u64 + ~rhs.u64;
        ans.u64++;
        if(ans.u64 < lhs.u64 && ans.u64 < rhs.u64)
            res.borrow = 1;
        res.result = ans.i64;
    }
    else if(is_u8<T>()) {
        lhs.u8 = _lhs; rhs.u8 = _rhs;
        ans.u8 = lhs.u8 + ~rhs.u8;
        ans.u8++;
        if(ans.u8 < lhs.u8 && ans.u8 < rhs.u8)
            res.borrow = 1;
        res.result = ans.u8;
    }
    else if(is_u16<T>()) {
        lhs.u16 = _lhs; rhs.u16 = _rhs;
        ans.u16 = lhs.u16 + ~rhs.u16;
        ans.u16++;
        if(ans.u16 < lhs.u16 && ans.u16 < rhs.u16)
            res.borrow = 1;
        res.result = ans.u16;
    }
    else if(is_u32<T>()) {
        lhs.u32 = _lhs; rhs.u32 = _rhs;
        ans.u32 = lhs.u16 + ~rhs.u16;
        ans.u32++;
        if(ans.u32 < lhs.u32 && ans.u32 < rhs.u32)
            res.borrow = 1;
        res.result = ans.u32;
    }
    else if(is_u64<T>()) {
        lhs.u64 = _lhs; rhs.u64 = _rhs;
        ans.u64 = lhs.u8 + ~rhs.u8;
        ans.u64++;
        if(ans.u64 < lhs.u64 && ans.u64 < rhs.u64)
            res.borrow = 1;
        res.result = ans.u64;
    }



    else if(is_f32<T>()) {
        res.result = _lhs - _rhs;
    }
    else if(is_f64<T>()) {
        res.result = _lhs - _rhs;
    }


    else {
        throw std::runtime_error(__function__);
    }


    return res;
}

} // namespace Op


