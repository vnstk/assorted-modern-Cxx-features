/*   (c)2024 Vainstein K.   */
#include "Josuttis--tracknew.hpp"
#include "common.h"
#include <new> //For   std::align_val_t

struct alignas(64) Foo {  // Overalignment for *all* objs of this type.  (That's 64B, not 64b.)
	char _ch;
	short _sh;
};

/* Qux, sizeof=4, but alignof=2.  Because the short must be aligned on 2.
Generally, alignof(SomeStructType) is max of the alignm requirements of its
datamembers.
*/
struct             Qux {
	uint8_t _ch;
	int16_t _sh;
};

struct             Zux {
	int16_t _sh;
	uint8_t _ch;
};


struct alignas(double) Bar {
	short _sh;
	bool _bo;
};


int main () {
	PRmsg("alignof(bool)=%zu , sizeof=%zu\n", alignof(bool), sizeof(bool));
	PRmsg("alignof(Foo)=%zu , sizeof=%zu\n", alignof(Foo), sizeof(Foo));
	PRmsg("alignof(Qux)=%zu , sizeof:%zu\n", alignof(Qux), sizeof(Qux));
	PRmsg("alignof(Zux)=%zu , sizeof:%zu\n", alignof(Zux), sizeof(Zux));

	static_assert(alignof(int16_t) == alignof(Qux));
	static_assert(2 == alignof(Qux));
	static_assert(64 == alignof(Foo));
	Zux *psimplyzux = new Zux;

	Foo fA;
	Foo fB;
	const long long delta_addrFoo = std::llabs((long long)&fB - (long long)&fA);
	fprintf(stderr, "|&fB-&fA| = %lld\n", delta_addrFoo);
	assert(64 <= delta_addrFoo);

	Qux qA;
	Qux qB;
	const long long delta_addrQux = std::llabs((long long)&qB - (long long)&qA);
	fprintf(stderr, "|&qB-&qA| = %lld\n", delta_addrQux);
	assert(2 <= delta_addrQux);

#if VER_ge17
	Qux *const pqC = new (std::align_val_t{256}) Qux; // Overalignment for just this allocat.
	Qux *const pqD = new (std::align_val_t{256}) Qux;
	const long long delta_addrQux_on_heap = std::llabs((long long)pqC - (long long)pqD);
	fprintf(stderr, "|pqC-pqD| = %lld\n", delta_addrQux_on_heap);
	assert(256 <= delta_addrQux_on_heap);
#endif
	fflush(stdout);
}
