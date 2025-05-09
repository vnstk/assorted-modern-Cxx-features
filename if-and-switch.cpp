/*   (c)2024 Vainstein K.   */
#include "common.h"
#include <map>
#include <string>
#include <time.h> // For clock()



int main ()
{
	std::map<int, std::string> m;

#if VER_ge17
	if (decltype(m)::const_iterator it = m.find(42); it != m.end()) {
		printf("Well dang, you did find it.\n");
	}
#endif
	//equiv to the old-fashioned "you was driving like a grandma"
	decltype(m)::const_iterator it = m.find(42);
	if (                                             it != m.end()) {
		printf("Well dang, you did find it.\n");
	}

#if VER_ge17
	// NB: may only declare vars of *one* type inside the if-cond.
#if 0
	if (int ii = 42; double dd = 3.5; bool bb = true; bb) {
	}
#endif
	// But may decl mult vars of that one type.
	if (int ii = 42, jj = 43, kk = ii+jj; ii < kk) {
	} else {
		ii *= -kk; // Look, scope extends all the way into else-block!
	}

	// Amusingly, can use the just-declared var as the if-cond.
	if (bool bb = m.empty(); bb) { printf("Made it\n"); }

	// Or can be altogether unrelated
	if (float ff = 3.5f; m.empty()) { printf("Made it, m empty, %.2f\n", ff); }
#endif


#if VER_ge17
	switch (int selector = clock() % 3; selector) {
		default: printf("Eh, whatev, selector = %d\n", selector);
	}
#endif

#if VER_ge23 || defined(__cpp_if_consteval)
	//consteval if ---TODO     https://en.cppreference.com/w/cpp/language/if
#endif
}

// Moved "constexpr if" examples into constexpr.cpp
