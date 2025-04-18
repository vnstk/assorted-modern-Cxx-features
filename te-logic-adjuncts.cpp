/*   (c)2024 Vainstein K.   */
#include "common.h"

#include <memory>
#include <limits>
#include <string>
#include <array>

#if VER_ge14
#	define ENA_void_ifCond(cond)         std::enable_if_t<(cond)>
#elif VER_eq11
#	define ENA_void_ifCond(cond)  typename std::enable_if<(cond)>::type
#endif

#if VER_ge14
#	define ENA_teArg_ifCond(cond,teArg)         std::enable_if_t<(cond),teArg>
#elif VER_eq11
#	define ENA_teArg_ifCond(cond,teArg)  typename std::enable_if<(cond),teArg>::type
#endif

#if VER_ge17
#	define ENA_void_ifMonadicTrai(trai,teArg)   std::enable_if_t<trai ## _v<teArg>>
#elif VER_ge14
#	define ENA_void_ifMonadicTrai(trai,teArg)         std::enable_if_t<trai<teArg>::value>
#elif VER_eq11
#	define ENA_void_ifMonadicTrai(trai,teArg)  typename std::enable_if<trai<teArg>::value>::type
#endif

#if VER_ge17
#	define ENA_void_ifDyadicTrai__other_1st(trai,other,teArg)   std::enable_if_t<trai ## _v<other,teArg>>
#elif VER_ge14
#	define ENA_void_ifDyadicTrai__other_1st(trai,other,teArg)         std::enable_if_t<trai<other,teArg>::value>
#elif VER_eq11
#	define ENA_void_ifDyadicTrai__other_1st(trai,other,teArg)  typename std::enable_if<trai<other,teArg>::value>::type
#endif

#if VER_ge17
#	define ENA_void_ifDyadicTrai__other_2nd(trai,teArg,other)   std::enable_if_t<trai ## _v<teArg,other>>
#elif VER_ge14
#	define ENA_void_ifDyadicTrai__other_2nd(trai,teArg,other)         std::enable_if_t<trai<teArg,other>::value>
#elif VER_eq11
#	define ENA_void_ifDyadicTrai__other_2nd(trai,teArg,other)  typename std::enable_if<trai<teArg,other>::value>::type
#endif


#if VER_ge17
#	define BOOL_isMonadicTrai(trai,teArg)   trai ## _v<teArg>
#else
#	define BOOL_isMonadicTrai(trai,teArg)         trai<teArg>::value
#endif


/* Branch on cpiletime-discernible properties of T, a te param.
*/
template<typename T>  /// "Embed" the condition in ret type; awkward. 
const ENA_void_ifCond(sizeof(T)>4) *
fSpurnsSmallDudes (void) {
	return (T*)42;       }
//
template<typename T> /// The 2-arg kind is slightly less awkward.
ENA_teArg_ifCond(sizeof(T)>4 ,T)
fStoreIntoNothingSmall (void) {
	return (T)42;             }
//
template<typename T, /// Least awkward is to add as extra te param.
         typename = ENA_void_ifCond(sizeof(T)>4)
> void fTakeNoSmallArgs (T targ) {
	fuPRtyp(targ);               }


/* Single perfect-fwdg ctor, instead of a ctor taking const T& + ctor a taking T&&.
*/
// see "C++ Templates" ed2 Josuttis et al., pp99-101 for *why*.
struct StrWra__a {
	std::string _s;
	template<typename STR, typename= ENA_void_ifDyadicTrai__other_2nd(std::is_convertible,STR,std::string) >
	explicit StrWra__a (STR&& fwdgRef) : _s(std::forward<STR>(fwdgRef)) { PRwhat(fwdgRef); fuPRtyp(fwdgRef); }
};
//
struct StrWra__b {
	std::string _s;
	template<typename STR, typename= ENA_void_ifDyadicTrai__other_1st(std::is_constructible,std::string,STR) >
	explicit StrWra__b (STR&& fwdgRef) : _s(std::forward<STR>(fwdgRef)) { PRwhat(fwdgRef); fuPRtyp(fwdgRef); }
};


/* Branch on cpiletime-discernible properties of a nontype te param.
*/
constexpr bool isMultipleOfThree (unsigned n) {
	return ! (n % 3U);
}
//// one way: partial specializat for "no" case, partial specializat for "yes" case.
template<unsigned SZ, bool = isMultipleOfThree(SZ)>
struct Helper__a // 1ry helper te
; 
template<unsigned SZ> // partial specializat.
struct Helper__a<SZ,false> { // impl for if SZ is not evenly divisible by 3
	Helper__a () { fuPRmsg("SZ=%u; chose SZ-isn't-evenly-divisible-by-3 impl.\n", SZ); }
};
template<unsigned SZ> // partial specializat.
struct Helper__a<SZ,true> { // impl for if SZ *is* evenly divisible by 3.
	Helper__a () { fuPRmsg("SZ=%u; chose SZ-*is*-evenly-divisible-by-3 impl.\n", SZ); }
};

//// anoth way: impl one case in the primary; partial specializat for the other case.
template<unsigned SZ, bool = isMultipleOfThree(SZ)>
struct Helper__b { // 1ry
	Helper__b () { fuPRmsg("SZ=%u; chose SZ-isn't-evenly-divisible-by-3 impl.\n", SZ); }
};
//
template<unsigned SZ> // partial specializat.
struct Helper__b<SZ,true> { // impl for if SZ *is* evenly divisible by 3
	Helper__b () { fuPRmsg("SZ=%u; chose SZ-*is*-evenly-divisible-by-3 impl.\n", SZ); }
};

void test__branchOnPropertiesOfNontypeTearg()
{	PRenteredFU;
	Helper__a<5>();
	Helper__a<9>();
	Helper__b<5>();
	Helper__b<9>();
	puts("==============================================================");
}


/* Selectively disable, w SFINAE, based on presence of partic
   member types and/or member methods.
*/
// Say wish ignore te arg which [A] has size_type membtype yet [B] lacks size() membfunc:

// This ver specifies cstraint [A] only, hence will be wrongfully selected.
//
template<typename T>
typename T::size_type len__a (T const& t) {
	return t.size();
}

// This ver specifies both cstraints, hence will not be wrongfully selected.
//
// Recall: value of comma operator expr is the Rmost operand; hence here, all
// operands to left of the Rmost comma are just *expressions which must be
// valid*.  Good idea to cast ea such expr to void, for [shudders] reasons.
// Josuttis 133
template<typename T>
auto len__b (T const& t) -> decltype(  (void)(t.size())  ,  T::size_type()  )
{                    //                      ^^^^^^^^^^     ^^^^^^^^^^^^^^
	return t.size(); //Oload won't be chosen if invalid.    Actual opd of decltype;
}                    //                                     NB, unevaluated!


#if VER_ge17 // Because these use "if constexpr".
/* Selectively disable specif statementsin (any!) func; func needn't be a te fu
   or membfunc of a te cla.
*/
template<typename T,
         typename = ENA_void_ifMonadicTrai(std::is_arithmetic,T) >
void test_selectivelyDisablingStmts__a (T targ) {
	fuPRlit("Hum ho");
	if constexpr(BOOL_isMonadicTrai(std::is_signed,T)) {
		PRlit("We confidently stand behind the signedness of all our arithm types!");
	}
}
// Needn't be te fu or membfunc of a te cla, even!
void test_selectivelyDisablingStmts__b (char ch) {
	fuPRlit("Ho hum");
	if constexpr(BOOL_isMonadicTrai(std::is_signed,char)) { // With <type_traits>
		PRlit("We confidently stand behind the signedness of our chars!  (one)");
	}
	if constexpr(std::numeric_limits<char>::is_signed) {    // With std::numeric_limits::*
		PRlit("We confidently stand behind the signedness of our chars!  (two)");
	}
}
#endif

#if VER_ge17 // Because uses "if constexpr".
template<int N, int I = 0>
void countUp() {
	if constexpr (I < N) {
		fuPRmsg("\tAt i=%d\n", I);
		countUp<N,I+1>();
	}
}
#endif


/* Selectively disable oloads of a func.  Non-disabled oloads must differ materially
   in their fu params, just like always with fu oloads.
*/

//NB: neither oload is primary; neither is the *default* choice.  Since
//    neither default, both must "= nullptr" --- recuse itself unless fitted.
template<typename T,
         ENA_void_ifMonadicTrai( std::is_integral ,T)  * = nullptr>
void selectOload__a (T const& t) {     // Dflt to false, ^^^^^^^^^
	fuPRlit("Chose impl for integral types");
}
template<typename T,
         ENA_void_ifMonadicTrai(!std::is_integral ,T)  * = nullptr>
void selectOload__a (T const& t) {     // Cast to ptr,^^^, to deal with ptr types.
	fuPRlit("Chose impl for non-integral types");
}

/// If not disabling all but one oload, oloads must differ in **fu** params (not te args!)


#if 0
template<typename T, unsigned QUES_COUNT>
void selectOload__b (std::array<T,3> const& arr) {
	std::string buf;
	for (auto arrElem : arr) {
		if (! buf.empty()) buf += ',';
		buf += std::to_string(arrElem);
	}
	for (unsigned j = 0; j < QUES_COUNT; ++j) buf += " ...?";
	fuPRlit(buf.c_str());
}
template<typename U, unsigned QUES_COUNT>
void selectOload__b (std::array<char,3> const& arr) {
	std::string buf;
	for (auto arrElem : arr) {
		if (! buf.empty()) buf += ' ';
		buf += std::to_string(arrElem);
	}
	if constexpr (BOOL_isMonadicTrai(std::is_integral,U)) {
		for (unsigned j = 0; j < QUES_COUNT; ++j) buf += " ...?";
	} else {
		for (unsigned j = 0; j < QUES_COUNT; ++j) buf += " ...!";
	}
	fuPRlit(buf.c_str());
}
#endif


void test__selectOload()
{	PRenteredFU;
	SAYeval(  selectOload__a<int>(100)               );
	SAYeval(  selectOload__a<std::string>("abc")     );
	SAYeval(  selectOload__a<const char *>("abc")    );
	// Now, rely on cpiler's deduction powers.
	SAYeval(  selectOload__a(100)     );
	SAYeval(  selectOload__a(1.0)     );
	SAYeval(  selectOload__a("abc")   );
	SAYeval(  selectOload__a((uintptr_t)(const char *)"abc")     );

#if 0
	std::array<float,3> farr = {1.11f, 2.22f, 3.33f};
	std::array<char,3> carr = {'x' , 'y' , 'z'};

	selectOload__b<double,5>(carr);
	selectOload__b<int   ,5>(carr);

	selectOload__b<float,5>(farr);
	selectOload__b<float,7>(farr); // oload not distinguishable by fu params!

	selectOload__b<int,5>(carr); // oload distinguishable by fu params.
	selectOload__b<int,7>(carr); // oload distinguishable by fu params.
	selectOload__b<double,5>(carr); // oload distinguishable by fu params.
#endif
	//
	puts("==============================================================");
}


/* Variable (not variadic!) te
*/
template<typename T>
constexpr char ArrPrintingElemSep = ',' ;
//
template<>
constexpr char ArrPrintingElemSep<float> = ' ' ;
//
template<typename T>
void printArr (std::array<T,3> const& arr) {
	std::string buf;
	for (auto arrElem : arr) {
		if (! buf.empty()) buf += ArrPrintingElemSep<T>;
		buf += std::to_string(arrElem);
	}
	fuPRlit(buf.c_str());
}


template <int... R>
constexpr int CpiletimeSum = (... + R); //using fold expression and variadic

void test__variables_te()
{	PRenteredFU;
	std::array<unsigned,3> uarr = {111U, 222U, 333U};
	std::array<double,3> darr = {1.11, 2.22, 3.33};
	std::array<float,3> farr = {1.11f, 2.22f, 3.33f};
	//
	printArr(uarr);
	printArr(darr);
	printArr(farr);
	
	int evalCpiletimeSum = CpiletimeSum<10,-30,7>;
	PRval(evalCpiletimeSum  ,"%d");
	puts("==============================================================");
}


/* std::conjunction , std::disjunction etc
*/
	// Variadic; simplest, with a te var.
template<typename T, typename... Ts>
constexpr bool allAreFunda = std::conjunction_v<std::is_fundamental<Ts>...>;

	// Variadic; another way, with several exclusive oloads --- all but one disabled.
template<typename T, typename... Ts>
std::enable_if_t<std::conjunction_v<std::is_fundamental<Ts>...> ,bool>
allFunda (T, Ts...)
{ fuPRlit("All funda.");  return true; }
template<typename T, typename... Ts>
std::enable_if_t<! std::conjunction_v<std::is_fundamental<Ts>...> ,bool>
allFunda (T, Ts...)
{ fuPRlit("Not all funda.");  return false; }

	// Not variadic, and somewhat silly.
template<typename T1, typename T2>
constexpr bool ref_and_unsi__a = std::conjunction<
			std::is_reference<T1>
		,	std::is_unsigned<T2>
	>::value;

void test__conjunction ()
{	PRenteredFU;
	static_assert(  allAreFunda<int,long,char>);
	static_assert(! allAreFunda<int,std::string,char>);

	if (allFunda(42,42L,'z')) { PRlit("Says all funda."); }
	if (! allFunda(42,42L,"nope")) { PRlit("Says *not* all funda."); }

	int i; unsigned long u; int *pi = &i; unsigned long& ru = u; int& ri = i;

	static_assert(  ref_and_unsi__a<decltype(ru), decltype(u)>  );
	static_assert(! ref_and_unsi__a<decltype(ru), decltype(i)>  );
	static_assert(! ref_and_unsi__a<decltype(pi), decltype(u)>  );
	static_assert(! ref_and_unsi__a<decltype(pi), decltype(i)>  );
	puts("==============================================================");
}



/* Misc nifty
*/
template<typename T, unsigned N>
constexpr std::size_t arrSize (const T(&)[N]) {   return N;   }
//
	// Call .size() meth of class, but only if said class has size_type membtype.
template<typename COLL>
constexpr typename COLL::size_type collSize (COLL const& coll) {   return coll.size();   }
//
void test__miscNifty()
{	PRenteredFU;
	float farr[23];
	assert(23U == arrSize(farr));
	static_assert(23U == arrSize(farr)); // Couldn't do this until made arrSize() constexpr.
#if VER_ge20
	auto farrFancy = std::to_array(farr);
	static_assert(23U == collSize(farrFancy));
#endif
	std::string sbunnies{"bunnies"};
	assert(7 == collSize(sbunnies));
	puts("==============================================================");
}


int main() {
	test__conjunction();
	test__branchOnPropertiesOfNontypeTearg();
	test__selectOload();

	std::allocator<float> allof;
//	auto retlen__a = len__a(allof);    // Cpiler error; icky.
//	auto retlen__b = len__b(allof);    // A *better* cpiler err.

#if VER_ge17 // Because these use "if constexpr".
	SAYeval(   test_selectivelyDisablingStmts__a( long() )        );
	SAYeval(   test_selectivelyDisablingStmts__a( uint64_t() )    );
	SAYeval(   test_selectivelyDisablingStmts__a( char() )        );
	SAYeval(   test_selectivelyDisablingStmts__b( char() )        );
#endif

	test__variables_te();
	test__miscNifty();


	SAYeval( (void)fSpurnsSmallDudes<uint64_t>() );
//	SAYeval( (void)fSpurnsSmallDudes<uint16_t>() ); //Err, disabled!

	SAYeval( (void)fStoreIntoNothingSmall<uint64_t>() );
//	SAYeval( (void)fStoreIntoNothingSmall<uint16_t>() );  //Err, disabled!

	SAYeval( fTakeNoSmallArgs(42LLU) );
//	SAYeval( fTakeNoSmallArgs(42LU) );
//	SAYeval( fTakeNoSmallArgs(42U) );


	SAYeval(   StrWra__a{"foo"}   ); //str literal ==> std::string xvalue ==> StrWra__a's move ctor
	std::string foo_strObj{"foo"};
	SAYeval(   StrWra__a{foo_strObj}   );           // std::string lvalue ==> StrWra__a's copy ctor
	SAYeval(   StrWra__a{std::move(foo_strObj)}   );// std::string prvalue ==> StrWra__a's move ctor.
	const std::string cfoo_strObj{"foo"};
	SAYeval(   StrWra__a{cfoo_strObj}   );
	const std::string& rcfoo_strObj = cfoo_strObj;
	SAYeval(   StrWra__a{rcfoo_strObj}   );
	const char* foo_cstr = "foo";
	SAYeval(   StrWra__a{foo_cstr}   );

	puts("-----------------------------------------");
	puts("-----------------------------------------");
	puts("-----------------------------------------");
	//
	SAYeval(   StrWra__b{"bar"}   ); //str literal ==> std::string xvalue ==> StrWra__b's move ctor
	std::string bar_strObj{"bar"};
	SAYeval(   StrWra__b{bar_strObj}   );           // std::string lvalue ==> StrWra__b's copy ctor
	puts("-----------------------------------------");

#if VER_ge17 // Because uses "if constexpr".
	countUp<5>();
#endif

}



/*
Josuttis p166
Josuttis p416

select fu, based on cstraints -- ch20

enable_if, all the tricks
std::conditional
given nontype param N, do foo(i) for i = 0..N
https://en.cppreference.com/w/cpp/types/void_t
*/
