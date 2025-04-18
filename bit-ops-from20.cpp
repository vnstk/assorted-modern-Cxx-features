/*   (c)2024 Vainstein K.   */
#include "common.h"
#include <type_traits>

// All these assume that x <= 1000

// finds the largest integral power of two not greater than the given value
//template<typename T,
//         typename std::enable_if<std::is_unsigned<T>::value>::type >
int bit_floor (const unsigned x)
{
	if (x & ~0b1111111111) // x > 0b1111111111 ?
		assert(0);
	if (x &  ~0b111111111) // x > 0b111111111 ?
		return   0x1 << 9;
	if (x & ~  0b11111111) // x > 0b11111111 ?
		return   0x1 << 8;
	if (x & ~   0b1111111) // x > 0b1111111 ?
		return   0x1 << 7;
	if (x & ~    0b111111) // x > 0b111111 ?
		return   0x1 << 6;
	if (x & ~     0b11111) // x > 0b11111 ?
		return   0x1 << 5;
	if (x & ~      0b1111) // x > 0b1111 ?
		return   0x1 << 4;
	if (x & ~       0b111) // x > 0b111 ?
		return   0x1 << 3;
	if (x & ~        0b11) // x > 0b11 ?
		return   0x1 << 2;
	if (x & ~         0b1) // x > 0b1 ?
		return   0x1 << 1;
	return       0x1 << 0;
}

// finds the smallest integral power of two not less than the given value    
//template<typename T,
//         typename std::enable_if<std::is_unsigned<T>::value>::type >
int bit_ceil (const unsigned x)
{
	return bit_floor(x) << 1;
}

// 1 + floor(lg x)
template<class T,
#if   VER_eq11
         typename = typename std::enable_if<std::is_unsigned<T>::value>::type
#elif VER_eq14
         typename = std::enable_if_t<std::is_unsigned<T>::value>
#elif VER_ge17
//         typename = std::enable_if_t<std::is_unsigned_v<T>>
         typename = std::enable_if_t<std::is_unsigned_v<T>>
#endif
>
int bit_width (const T x)
//int bit_width (const unsigned x)
{
	if (x& ~0b1111111111) // x > 0b1111111111 ?
		assert(0);
	if (x & ~0b111111111) // x > 0b111111111 ?
		return 10;
	if (x & ~ 0b11111111) // x > 0b11111111 ?
		return 9;
	if (x & ~  0b1111111) // x > 0b1111111 ?
		return 8;
	if (x & ~   0b111111) // x > 0b111111 ?
		return 7;
	if (x & ~    0b11111) // x > 0b11111 ?
		return 6;
	if (x & ~     0b1111) // x > 0b1111 ?
		return 5;
	if (x & ~      0b111) // x > 0b111 ?
		return 4;
	if (x & ~       0b11) // x > 0b11 ?
		return 3;
	if (x & ~        0b1) // x > 0b1 ?
		return 2;
	return 0x1;
}

int main() {
	int z = bit_width((unsigned)0xFF);
#if 0
	assert(bit_width<unsigned>(0xFF) == 8);
	assert(bit_width(0x7F) == 7);
	assert(bit_width(0x74) == 7);
	assert(bit_width(0x71) == 7);
	assert(bit_width(0xF0) == 8);
	assert(bit_width(0x80) == 8);
	assert(bit_width(0x8) == 4);
	assert(bit_floor(0xAF) == 0x80);
	assert(bit_ceil(0x6F) == 0x80);
#endif
}

