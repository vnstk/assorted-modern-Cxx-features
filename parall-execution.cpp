/*   (c)2025 Vainstein K.   */
//XXX Must compile with  -fexceptions
#include "common.h"
#include "simple-timer.h"
#include <vector>
#include <algorithm>
#include <numeric>
#include <execution>  //For std::execution::* and std::is_execution_policy
#include <cmath>      //For sqrt()

//adapted from lib/parforeach.cpp in Josuttis "C++17" code examp
struct Elem {
	double _inp;
	double _sqrtOf;
};
#define N_Elem 1000

template<size_t NElems, typename TexePolicy>
void labor (TexePolicy exePolicy) {
	static_assert(std::is_execution_policy<TexePolicy>::value);
	static_assert(std::is_execution_policy_v<TexePolicy>);

	//adapted from lib/parforeach.cpp in Josuttis "C++17" code examp
	std::vector<Elem> vec;
	vec.reserve(NElems);
	for (int i=0; i<NElems; ++i) {
		vec.push_back(Elem{i * 4.37, 0.0F});
	}
	Timer t(currFU);

try {
	std::for_each(exePolicy,
           vec.begin(), vec.end(),
           [](auto& elem) {
             elem._sqrtOf = std::sqrt(elem._inp);
           });
} catch (const std::bad_alloc&) { exit(4); }
}


template<size_t NElems>
void test__basic()
{

#if VER_ge17
	labor<NElems>(std::execution::seq);       //ought deduce TexePolicy = std::execution::sequenced_policy
	labor<NElems>(std::execution::par);       //ought deduce TexePolicy = std::execution::parallel_policy
	labor<NElems>(std::execution::par_unseq); //ought deduce TexePolicy = std::execution::parallel_unsequenced_policy
#endif
#if VER_ge20
	labor<NElems>(std::execution::unseq);     //ought deduce TexePolicy = std::execution::unsequenced_policy
#endif


}

int main ()
{
	Timer t{"maint"};
	test__basic<10>();
	test__basic<100>();
	test__basic<1000>();
	test__basic<10000>();
}


#if 0

pmr::..... ??
#endif
