/*   (c)2024 Vainstein K.   */
#include "common.h"

struct alignas(64) Foo {
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
	fprintf(stderr, "alignof(bool)=%lu , sizeof=%lu\n", alignof(bool), sizeof(bool));
	fprintf(stderr, "alignof(Foo)=%lu , sizeof=%lu\n", alignof(Foo), sizeof(Foo));
	fprintf(stderr, "alignof(Qux)=%lu , sizeof:%lu\n", alignof(Qux), sizeof(Qux));

	assert(32 == alignof(Qux));
	assert(64 == alignof(Foo));

	Foo fA;
	Foo fB;
	fprintf(stderr, "&fB-&fA = %ldn", (&fB - &fA));
	assert(64 == (&fB - &fA));
	Qux qA;
	Qux qB;
	assert(32 == (&qB - &qA));
}
