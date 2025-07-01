/*   (c)2024 Vainstein K.   */
#include "common.h"
#include <compare>

struct Foo {
	const float _f;
	const int _i;
	Foo (float f, int i) : _f(f), _i(i) {}
#if VER_ge20
	// Weird, op<=> must be either non-static, or non-member.
	// And "friend" somehow makes it non-member??
	friend constexpr std::strong_ordering operator<=> (const Foo& lhs, const Foo& rhs) {
		if (lhs._i < rhs._i || (lhs._i == rhs._i && lhs._f < rhs._f)) {
			return std::strong_ordering::less;                        }
		if (lhs._i > rhs._i || (lhs._i == rhs._i && lhs._f > rhs._f)) {
			return std::strong_ordering::greater;                     }
		return std::strong_ordering::equal;
	}

	// May not declare more than 1 flavor of operator<=>
/*
	friend constexpr std::partial_ordering operator<=> (const Foo& lhs, const Foo& rhs) {
		if (lhs._i < rhs._i || (lhs._i == rhs._i && lhs._f < rhs._f)) {
			return std::partial_ordering::less;                        }
		if (lhs._i > rhs._i || (lhs._i == rhs._i && lhs._f > rhs._f)) {
			return std::partial_ordering::greater;                     }
		return std::partial_ordering::equivalent;
	}
*/
#endif
};

struct Bar {
	const float _f;
	const int _i;
	Bar (float f, int i) : _f(f), _i(i) {}
#if VER_ge20
//	constexpr std::strong_ordering operator<=> (const Bar& rhs) const =default;
//	constexpr std::weak_ordering operator<=> (const Bar& rhs) const =default;
		// floatpt types only have partial_ordering defined on them, hence cpiler
		// won't "=default"-generate either of those if class has any floatpt ivars.
	constexpr std::partial_ordering operator<=> (const Bar& rhs) const =default;
#endif
};


int main() {

	Foo fA{3.3f,     42};
	Foo fB{3.3f,     52};
	Foo fC{3.30001f, 32};
	Foo fD{3.3f,     42};
#ifdef VER_ge20
	PRmsg("fA == fC: %c\n", fA <= fA ? 'T':'F');
	PRmsg("fA <  fC: %c\n", fA <  fC ? 'T':'F');
	PRmsg("fB >  fD: %c\n", fB >  fD ? 'T':'F');
#endif
	puts("----------------------------------------------");

	Bar bA{3.3f,     42};
	Bar bB{3.3f,     52};
	Bar bC{3.30001f, 32};
	Bar bD{3.3f,     42};
#ifdef VER_ge20
	PRmsg("bA == bC: %c\n", bA <= bA ? 'T':'F');
	PRmsg("bA <  bC: %c\n", bA <  bC ? 'T':'F');
	PRmsg("bB >  bD: %c\n", bB >  bD ? 'T':'F');
#endif
}
