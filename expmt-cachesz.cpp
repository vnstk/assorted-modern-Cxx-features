/*   (c)2025 Vainstein K.   */
#include "common.h"
#include "simple-timer.h"
#include <new>
#include <thread>

#if defined(__cpp_lib_hardware_interference_size) // In other words, VER_ge17
#	define L1_Dcache_LINE_SIZE std::hardware_destructive_interference_size
#else
#	define L1_Dcache_LINE_SIZE 64
#endif

struct FalseSharing { // a and b both 8B, hence will go into same cache line
	uint64_t _a;
	uint64_t _b;
};
static FalseSharing falseSharer;

struct ProperSharing {
	alignas(L1_Dcache_LINE_SIZE)  uint64_t _a;
	alignas(L1_Dcache_LINE_SIZE)  uint64_t _b;
};
static ProperSharing properSharer;

constexpr int Nrounds = 1000000;
void repeatedlyTouch (uint64_t *const p)
{ for (int i = 0; i < Nrounds; ++i) *p += 3U; }

void test__false_sharing()
{	PRenteredFU;
	{	Timer t{"false sharing"};
		std::thread thdA{repeatedlyTouch, &falseSharer._a};
		std::thread thdB{repeatedlyTouch, &falseSharer._b};
		thdA.join();
		thdB.join();
	}
	{	Timer t{"proper sharing"};
		std::thread thdA{repeatedlyTouch, &properSharer._a};
		std::thread thdB{repeatedlyTouch, &properSharer._b};
		thdA.join();
		thdB.join();
	}
}


template<size_t NBytesPadding>
struct Padded {
	uint64_t _a;
	char _padding[NBytesPadding];
	uint64_t _b;
};

template<size_t NBytesPadding>
void touchAlternating (Padded<NBytesPadding>& x) {
	for (int i = 0; i < Nrounds; ++i) {
		x._a += 3;
		x._b += 3;
	}
}

void test__fitting_into_cacheLine ()
{	PRenteredFU;
	//TODO: impl a more convincing experiment for the ...constructive aspect.
	{	Padded<40> x;
		static_assert(sizeof x == 56);
		Timer t{"tot sz=56B, on-stack"};
		touchAlternating(x);
	}

	{	Padded<48> x;
		static_assert(sizeof x == 64);
		Timer t{"tot sz=64B, on-stack"};
		touchAlternating(x);
	}

	{	Padded<56> x;
		static_assert(sizeof x == 72);
		Timer t{"tot sz=72B, on-stack"};
		touchAlternating(x);
	}

	{	Padded<40> *px = new Padded<40>;
		static_assert(sizeof *px == 56);
		Timer t{"tot sz=56B, on-heap"};
		touchAlternating(*px);
		delete px;
	}

	{	Padded<48> *px = new Padded<48>;
		static_assert(sizeof *px == 64);
		Timer t{"tot sz=64B, on-heap"};
		touchAlternating(*px);
		delete px;
	}

	{	Padded<56> *px = new Padded<56>;
		static_assert(sizeof *px == 72);
		Timer t{"tot sz=72B, on-heap"};
		touchAlternating(*px);
		delete px;
	}


}


int main()
{
	PRmsg("This runtime supports max %u concurrent threads\n",
		  std::thread::hardware_concurrency());

#if defined(__cpp_lib_hardware_interference_size) // In other words, VER_ge17
	// These will both print size of L1 d-cache.  Which will be 64B.
	PRmsg("Minimum offset between two objects to avoid false sharing = %zu\n",
	      std::hardware_destructive_interference_size);
	PRmsg("Maximum size of contiguous memory to promote true sharing = %zu\n",
	      std::hardware_constructive_interference_size);
#endif

	test__false_sharing();
	test__fitting_into_cacheLine();
}

