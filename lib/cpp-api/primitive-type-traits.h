#pragma once

static_assert(sizeof(char) == 1);
static_assert(sizeof(unsigned char) == 1);
static_assert(sizeof(short) == 2);
static_assert(sizeof(unsigned short) == 2);
static_assert(sizeof(int) == 4);
static_assert(sizeof(unsigned int) == 4);
static_assert(sizeof(long int) == 8);
static_assert(sizeof(unsigned long int) == 8);


//
// signed integer types
//

template<typename T> constexpr bool is_i8(void) { return false; }
template<> constexpr bool is_i8<char>(void) { return true; }

template<typename T> constexpr bool is_i16(void) { return false; }
template<> constexpr bool is_i16<short>(void) { return true; }

template<typename T> constexpr bool is_i32(void) { return false; }
template<> constexpr bool is_i32<int>(void) { return true; }

template<typename T> constexpr bool is_i64(void) { return false; }
template<> constexpr bool is_i64<long int>(void) { return true; }



//
// unsigned integer types
//

template<typename T> constexpr bool is_u8(void) { return false; }
template<> constexpr bool is_u8<unsigned char>(void) { return true; }

template<typename T> constexpr bool is_u16(void) { return false; }
template<> constexpr bool is_u16<unsigned short>(void) { return true; }

template<typename T> constexpr bool is_u32(void) { return false; }
template<> constexpr bool is_u32<unsigned int>(void) { return true; }

template<typename T> constexpr bool is_u64(void) { return false; }
template<> constexpr bool is_u64<unsigned long int>(void) { return true; }



//
// floating-point types
//

template<typename T> constexpr bool is_f32(void) { return false; }
template<> constexpr bool is_f32<float>(void) { return true; }

template<typename T> constexpr bool is_f64(void) { return false; }
template<> constexpr bool is_f64<double>(void) { return true; }
