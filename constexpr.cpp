/*   (c)2024 Vainstein K.   */
#include "common.h"
#include <string.h>
#include <initializer_list>
#include <vector>
#include <algorithm>
#include <array>


//==================== constexpr vars ==============================

constexpr unsigned ulim = 42;
///constexpr unsigned& ref1 = 42U; // No!
constexpr const unsigned& ref2 = 42U;
static constexpr const unsigned& ref3 = 42U;
///static constexpr       unsigned& ref4 = 42U;   //No!

#if VER_ge20
constexpr size_t funny_0 (void) {
	/* main problem is that we cannot easily store the output in a constexpr string or vector
		--- https://www.cppstories.com/2021/constexpr-vecstr-cpp20/
	*/
	// vector initialized at cpiletime ...????
	std::vector<int> vv({555, 77777, 9999999});
	return vv.size();
}
constexpr size_t funny_1 (const std::initializer_list<int> ilistArg) {
	std::vector<int> vv(ilistArg);
	return vv.size();
}
constexpr size_t funny_2 (void) {
//	constexpr std::initializer_list<int> jlist{555, 77777, 9999999, 1111111111};  //Err!!
	std::initializer_list<int> ilist{555, 77777, 9999999, 1111111111};
	return funny_1(ilist);
}
//
constexpr std::initializer_list<int> global_ilist{22,444,6666,88888,101010,222};
//
void test__the_funnies () {
	PRenteredFU;
	static_assert(3LU == funny_0()); // cpiletime
	       assert(3LU == funny_0()); // runtime
	static_assert(4LU == funny_2()); // cpiletime
	       assert(4LU == funny_2()); // runtime
	const std::initializer_list<int> local_ilist{22,444,6666,88888,101010};
	       assert(5LU == funny_1(local_ilist)); // runtime
	static_assert(6LU == funny_1(global_ilist)); // cpiletime
}
#endif


//==================== constexpr funcs ==============================

class Foo {
public:
	const int _x;
	int _y;
	Foo (int x) : _x(x), _y(42) { }
	constexpr int bar (void) const { return _x; }
	constexpr int qux (void)       { return _y; } // Thought this'd be illegal in C++11??
};

#if VER_ge14
// In C++11, no local var decl
constexpr int f000 (void) {
	int i = 42;
	if (strlen("bunnies") > ulim)
	return 33;
	else
	return 77;
}
constexpr int zz = f000();
#endif


#if VER_ge17 // only get "if constexpr" in 17
template<typename T>
auto get_value(T t)
{
    if constexpr (std::is_pointer_v<T>)
        return *t; // deduces return type to int for T = int*
    else
        return t;  // deduces return type to int for T = int
}
#endif
// , sorta equiv to
#if NULL
template<typename T>
T fugget_value(T t)
{
	static_assert(! std::is_pointer_v<T>);
	return t + 42;
}
template<typename TPtr>
auto fugget_value(TPtr t)
{
	static_assert(std::is_pointer_v<TPtr>);
	return *t + 42;
}
#endif


int main() {
#if VER_ge20
	test__the_funnies();
#endif

	const Foo fooa{77};
//	PRwhat(fooa);
	Foo       foob{77};
	int reta = fooa.bar();
	int retb = foob.bar();
#if VER_eq11
	// 14 on, constexpr implies const.
	int retc = fooa.qux();
#endif
	int retd = foob.qux();

	Foo& x0 = foob;
	const Foo& x1 = fooa;
	const Foo& x2 = foob;
//	PRwhat(x2);

	const float& crf = 3.14159F;

	std::vector<Foo> vfoo;
	vfoo.emplace_back(45U);
	//
	auto unaryPred1 =
	              [](const Foo& refFoo){return refFoo._x > refFoo._y;};
	(void) std::count_if(vfoo.cbegin(), vfoo.cend(), unaryPred1);
	//
#if VER_ge17 // constexpr lambdas
	constexpr auto unaryPred2 =
	              [](const Foo& refFoo){return refFoo._x > refFoo._y;};
	(void) std::count_if(vfoo.cbegin(), vfoo.cend(), unaryPred2);
#endif

	          int Nz = 42;
	const     int Na = Nz;
	const     int Nb = 42;
	constexpr int Nc = 42;
//	std::array<float,Na> arrayTNa;    //Err!!  Because Na populated runtime.
	std::array<float,Nb> arrayTNb;
	std::array<float,Nc> arrayTNc;

}
