/*   (c)2024 Vainstein K.   */
#include "common.h"
#include <stdlib.h> // for srand() and rand()
#include <algorithm>
#include <list>
#include <numeric> // Part of the algorithms lib is in <numeric> and not in <algorithm>.
#include <string>
#include <vector>


struct NumStr {
	/* If ivars were const, std::accumulate wouldn't work; apparently it uses the copy
	   assignm op.  Default-initializes a temporary and *then* assigns into it, I guess.
	   Why not just wait until have the args for non-dflt ctor??
	*/
	unsigned    _num;
	std::string _str;
	NumStr (unsigned num, std::string&& str) noexcept
		: _num(num), _str(std::move(str)) {}
};
NumStr foobarize (NumStr const& a, NumStr const& b) {
	char buf[128];
	sprintf(buf, "%s%u|%u%s", a._str.c_str(), a._num, b._num, b._str.c_str());
	return NumStr(a._num * b._num, std::string(buf));
}

struct CatAdder {
	std::string _s;
	explicit CatAdder (std::string&& s) : _s(std::move(s)) {}
	float operator()(unsigned& u) {
		(_s += "|") += std::to_string(u);
		u += 10;
		return u * 1.1F;
	}
};


/* Fold expressions: apply binary operator to all elems of a parampack.
*/
#if VER_ge17
template<typename... Ts> // <--- te params
decltype(auto) fold_subA (Ts... args) { // <--- fu params
	fuPRmsg("Size of te parampack: %zu\n", sizeof...(Ts));
	fuPRmsg("Size of fu parampack: %zu\n", sizeof...(args));
	return (... - args);                 // unary left fold   ---  "left-associative"
}
template<typename... Ts>
decltype(auto) fold_subB (Ts... args) {
	return (args - ...);                 // unary right fold  ---  "right-associative"
}
template<typename... Ts>
decltype(auto) fold_subC (Ts... args) {
	fuPRmsg("Size of te parampack: %zu\n", sizeof...(Ts));
	return (100 - ... - args);           // binary left fold  ---   "left-associative"
}
template<typename... Ts>
decltype(auto) fold_subD (Ts... args) {
	fuPRmsg("Size of fu parampack: %zu\n", sizeof...(args));
	return (args - ... - 100);           // binary right fold  ---  "right-associative"
}

template<typename... Ts>
decltype(auto) fold_lshiftB (Ts... args) {
	return (args << ...);                // unary right fold
}
template<typename... Ts>
decltype(auto) fold_lshiftD (Ts... args) {
	return (args << ... << 3LU);         // binary right fold
}

template<typename... Ts>
decltype(auto) fold_addC (Ts... args) {
	return (0 + ... + args);
}
#endif


void test__foldExprs () {
	PRenteredFU;
#if VER_ge17
	SAYevalPRret(  fold_subA(20,7,9)  ,"%d");   //   (20-7)-9
	SAYevalPRret(  fold_subB(20,7,9)  ,"%d");   //    20-(7-9)
	SAYevalPRret(  fold_subC(20,7,9)  ,"%d");   //   ((100-20)-7)-9
	SAYevalPRret(  fold_subD(20,7,9)  ,"%d");   //   20-(7-(9-100))
	//
	SAYevalPRret(  fold_lshiftB(7LLU,5LLU,3LLU)   ,"%llu");   //     7<<(5<<3)
	SAYevalPRret(  fold_lshiftD(7LLU,5LLU)        ,"%llu");   //     7<<(5<<3)
	static_assert(std::is_same_v<decltype(fold_lshiftB(7LLU,3LLU)), long long unsigned>);
	//
	// Big advantage of the binary folds is won't barf if called with *no* args:
	SAYevalCHKret(  fold_addC()  ,"%d", 0);
	// If binary fold with initial-val clause called with no args, get back the initial-val.
	SAYevalCHKret(  fold_subC()  ,"%d", 100);
	SAYevalCHKret(  fold_subD()  ,"%d", 100);/* Ret type depends on how initial-val literal
	                                             was written; if "100L", would return long. */
	//
	// If a unary fold called with just *one* arg, ought to simply return it:
	SAYevalCHKret(  fold_subA(42)  ,"%d", 42);
	SAYevalCHKret(  fold_subB(42)  ,"%d", 42);
#endif
}

int mk_pseudorandomInt ()
{
    static bool haveSeeded{false};
    if (! haveSeeded) {
        srand(42U);
        haveSeeded = true;
    }
	return rand();
}

void test__generate ()
{	PRenteredFU;
//
	std::vector<int> numbers(20); // All dflt-initialized to 0.
	auto isZero = [](int x) -> bool { return 0==x; };
	SAYevalCHKretBOOL(   std::all_of(numbers.cbegin(), numbers.cend(), isZero)   ,true);
	std::generate(numbers.begin(), numbers.end(), mk_pseudorandomInt);
	SAYevalCHKretBOOL(   std::none_of(numbers.cbegin(), numbers.cend(), isZero)   ,true);
//
	std::vector<std::string> strings{"aa", "bb", "cc", "dd", "ee"};
	assert("cc" == strings[2]);
#if VER_ge17
	decltype(strings)::const_iterator cit =
		std::generate_n(strings.begin(), 3U,
		                []()->std::string { return std::to_string(mk_pseudorandomInt()); });
	assert("dd" == *cit);
	assert("cc" != strings[2]);
#endif
	PRlit(strings[2].c_str());
}

void test__for_each ()
{	PRenteredFU;
//
	std::vector<unsigned> vuX{5,7,9};
	std::for_each(vuX.begin(), vuX.end(), [](unsigned& u){ u+=10U; });
	assert(19 == vuX[2]);
	//
	std::vector<unsigned> vuY{5,7,9};
	CatAdder caPre{"Z"};
	CatAdder caPost = std::for_each(vuY.begin(), vuY.end(), caPre);
	assert(19 == vuY[2]);
	assert(caPost._s == "Z|5|7|9");
	//
	std::vector<unsigned> vuZ{5,7,9,1111};
#if VER_ge17
	decltype(vuZ)::const_iterator citZ = std::for_each_n(vuZ.begin(), 3U,
	                                         [](unsigned& u){ u+=10U; });
	assert(1111U == *citZ);
	assert(19 == vuZ[2]);
#endif
}

void test__transform ()
{	PRenteredFU;
//
	std::vector<unsigned> vu{5,7,9};
	std::list<unsigned> liu{103, 1003, 10003, 100003};
	decltype(liu)::const_iterator third = std::next(liu.cbegin() ,2U);
	assert(10003 == *third);
	decltype(liu)::const_iterator firstNontransformed =
		std::transform(vu.cbegin(), vu.cend(), liu.begin(), [](unsigned u){ return u+10; });
	assert(100003 == *firstNontransformed);
	assert(10003 != *third);
	assert(19    == *third);
	assert(9 == vu[2]); // Confirm that input range is untouched --- unlike with for_each.
}

void test__accumulate ()
{	PRenteredFU;
//
	// Oload not taking a dyadic callable, just does addition.
	std::vector<unsigned> vn{5,7,9};
	unsigned sum = std::accumulate(vn.cbegin(), vn.cend(), 20U);
	assert(/*5 + 7 + 9 + 20*/41 == sum);
	//
	// Additions means operator+(), which of course could be oloaded for elem type.
	std::vector<std::string> vs{"aa","bb","cc"};
	std::string cat = std::accumulate(vs.cbegin(), vs.cend(), std::string("dd"));
	PRlit(cat.c_str());
	assert(std::string("ddaabbcc") == cat); // Result is *not* "aabbccdd"
//
	std::vector<NumStr> vns{ {5,"aa"} , {7,"bb"} , {9,"cc"} };
	const NumStr total = std::accumulate(vns.cbegin(), vns.cend(), NumStr{3,"dd"} ,foobarize);
	PRmsg("total _num=%u _str=\"%s\"\n", total._num, total._str.c_str());
	// Prints  str="dd3|5aa15|7bb105|9cc"
}

void test__reduce () // How... is this different from accumulate??
{	PRenteredFU;
#ifdef VER_ge17
	std::vector<unsigned> vu{5,7,9};
	unsigned sum00 = std::reduce(vu.cbegin(), vu.cend());
	assert(21U == sum00);

	unsigned sum01 = std::reduce(vu.cbegin(), vu.cend(), 20U);
	assert(41U == sum01);

	std::vector<std::string> vs{"aa","bb","cc"};
	std::string cat = std::reduce(vs.cbegin(), vs.cend(), std::string("dd"));
	PRlit(cat.c_str());
	assert(std::string("ddaabbcc") == cat);

	std::vector<NumStr> vns{ {5,"aa"} , {7,"bb"} , {9,"cc"} };
	const NumStr total = std::reduce(vns.cbegin(), vns.cend(), NumStr{3,"dd"} ,foobarize);
	PRmsg("total _num=%u _str=\"%s\"\n", total._num, total._str.c_str());
	// Prints  str="dd3|5aa15|7bb105|9cc"
#endif
}


int main () {
	test__accumulate();
    test__foldExprs();
	test__for_each();
	test__generate();
	test__reduce();//TODO: test with other ExecutionPolicy ??
	test__transform();
}
