#include "shared.cpp" // So needn't compile shared.o as a separate step.
#include <new>


int main()
{
	Timer tee{"primary"};

//	std::chrono::high_resolution_clock::rep foo;
//	PRarithType(foo);

#if defined(__cpp_lib_hardware_interference_size)
	// These will both print size of L1 d-cache.  Which will be 64B.

	PRmsg("Minimum offset between two objects to avoid false sharing = %zu\n",
	      std::hardware_destructive_interference_size);

	PRmsg("Maximum size of contiguous memory to promote true sharing = %zu\n",
	      std::hardware_constructive_interference_size);
#endif
}

