/*   (c)2024 Vainstein K.   */
#include "common.h"
#include <functional> // For std::hash

struct Flarp {
	unsigned _u{42U};
};

#if VER_ge20
template<typename T>
concept Hashable = requires(T a)
{
    { std::hash<T>{}(a) } -> std::convertible_to<std::size_t>;
};
// 4 equiv ways to apply the above constraint:
template<Hashable T>
void haf0(T targ) {}
//
template<typename T> requires Hashable<T>
void haf1(T targ) {}
//
template<typename T>
void haf2(T targ) requires Hashable<T> {}
//
void haf3(Hashable auto targ) {}
#endif

// The 4th form, as used with haf3() above, also takes built-in concepts.
#if VER_ge20
bool isPositive (std::integral auto x) {
	return x > 0;
}
#endif



int main() {
#if VER_ge20
	haf0(42U); // OK, hash() defined.
	Flarp flarp;
//	haf0(flarp); // Err!  Does not satisfy Hashable.
#endif

	SAYevalCHKretBOOL(isPositive(42L)  ,true);
	SAYevalCHKretBOOL(isPositive(-7L)  ,false);
}
