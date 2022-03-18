#ifndef MY_LIB
#define MY_LIB

#include <cstdint>

typedef uint64_t u64;
typedef uint32_t u32;
typedef uint16_t u16;
typedef uint8_t u8;


template <class T>
inline T min(T a, T b) {return a < b ? a : b;};

template <class T>
inline T max(T a, T b) {return a > b ? a : b;};

template <class T>
inline u32 size_bits() {return sizeof(T)<<3;}

template <class T>
inline T max_value(u32 w) {return w==size_bits<T>() ? T(-1) : (T(1)<<w)-1;}

template <class T>
inline void swap(T& a, T& b) {a ^= b; b ^= a; a ^= b;}

#endif
