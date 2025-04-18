/*   (c)2024 Vainstein K.   */
#include "common.h"
#include <utility> // For std::make_integer_sequence
#include <vector>
#include <tuple>
#include <numeric> // For std::accumulate


/* Operate on arb number of args; instead of a special "base-case" oload,
   test cardinality of parampack-expanded argslist with sizeof...() oper.
*/
template<typename T, typename... Args>
T fsum__b (T first, Args... rest) {
	if constexpr (sizeof...(rest)  == 0) // base case
		return first;
	else
		return first + fsum__b(rest...);
}


/* Operate on arb number of args; using explicit "operate on the
   car & recurse on the cdr" technique.
*/
int fsum__a (void) {//base case
	return 0;
}
template<typename T1, typename... Ts>            /// binary op.
#if VER_ge14
decltype(auto) fsum__a (T1 first, Ts... rest) {
#else
         T1    fsum__a (T1 first, Ts... rest) {
#endif
	return first + fsum__a(rest ...);
}
// With fold exprs, these 2 fsum__a() oloads can be replaced with just fold_addC().


/* Operate on parampack *as a whole*.
*/
template<typename... Ts>
long fsumDoubled (Ts const&... args) {
	return (long) fsum__a(args + args ...);
}


void test__fsum () {
	PRenteredFU;

	auto retX = fsum__b(4, true, 7.0f);
	PRtyp(retX); // int
	PRval((double)retX ,"%.3f");
//	printf("(Ln%d) retZ = %.3f\n",__LINE__, (float)retZ);

	auto retZ = fsum__a(4, true, 7.0f);
	PRtyp(retZ); // float!!
	PRval((double)retZ ,"%.3f");
//	printf("(Ln%d) retZ = %.3f\n",__LINE__, (float)retZ);
	//
	SAYevalPRret( fsumDoubled(11, 13)   ,"%ld"  );  // 22 + 26 = 48
}


void prSizeof__a () {//base case
	fuPRlit("base case");
}
template<typename T, typename... Ts>             /// unary op.
void prSizeof__a (T first, Ts... rest) {
	fuPRmsg("sizeof is %lu\n", sizeof(first));
	prSizeof__a(rest ...);
}

// Variation of above: non-parampack oload does the actual work, parampack oload directs.
//NB: fu te oload *without* parampack is prioritized (preferred).
template<typename T>
void prSizeof__b (T justTheOne) {
	fuPRmsg("sizeof is %lu\n", sizeof(justTheOne));
}
template<typename T, typename... Ts>
void prSizeof__b (T first, Ts... rest) {
	prSizeof__b(first);
	prSizeof__b(rest ...);
}

void test__prSizeof () {
	PRenteredFU;
	prSizeof__a(3, 3LU, 3LLU, (short)3);
		/*Print:
			prSizeof__a(T, Ts ...) [with T = int; Ts = {long unsigned int, long long unsigned int, short int}]
			prSizeof__a(T, Ts ...) [with T = long unsigned int; Ts = {long long unsigned int, short int}]
			prSizeof__a(T, Ts ...) [with T = long long unsigned int; Ts = {short int}]
			prSizeof__a(T, Ts ...) [with T = short int; Ts = {}]
			prSizeof__a()
		*/
	puts("----------");
	prSizeof__b(3, 3LU, 3LLU, (short)3);
		/*Print:
			prSizeof__b(T) [with T = int]
			prSizeof__b(T) [with T = long unsigned int]
			prSizeof__b(T) [with T = long long unsigned int]
			prSizeof__b(T) [with T = short int]
		*/
}



/* Variadic indices, to select partic elems in an indexable sequence.
*/
template<typename T>
void prIntegralVal (T one) {
	fuPRmsg("\tval=%ld\n", (long)one);
}
template<typename T, typename... Ts>
void prIntegralVal (T first, Ts... rest) {
	prIntegralVal(first);
	prIntegralVal(rest ...);
}
//
template<typename SEQ, typename... Idx>
void prVal_seq_elems_byIndex__a (SEQ const& seq, Idx... idx) {
	prIntegralVal(seq[idx] ...); // "idx..." unpacks the fu-param parampack.
}
//
template<std::size_t... Idx, typename SEQ>
void prVal_seq_elems_byIndex__b (SEQ const& seq) {
	prIntegralVal(seq[Idx] ...); // "Idx..." unpacks the te-param parampack.
}
//
void test__prVal_seq_elems_byIndex() {
	PRenteredFU;
	std::vector<int> vi{42,337,-6,999,0};  //Indices 3,1,2  would be vals  99,337,-6.
	//
	PRlit("prVal_seq_elems_byIndex__a");
	       prVal_seq_elems_byIndex__a(vi, 3,1,2);
	//
	PRlit("prVal_seq_elems_byIndex__b");
	       prVal_seq_elems_byIndex__b<3,1,2>(vi);
}

/*
#include <iostream>
template<typename T, T... intseq>
void bar (void) {//std::integer_sequence<T, intseq...> intseqArg) {
    ((std::cout << intseq << ' '), ...);
}
*/
//	fuPRmsg("OK; |Idx| = %lu\n", intseqArg.size());
//	printf("Idx = %ld\n", (long) std::integral_constant<T, Idx>() ... );

template<typename T>
void qoo (void) {
	fuPRlit("xxx"); //fu==>    qoo() [with T = std::integer_sequence<long long unsigned int, 0, 1, 2, 3, 4, 5, 6>]
}

template<typename T>
void zoo (T t) {
	fuPRlit("yyy"); //fu==>     zoo(T) [with T = std::tuple<int, int, int>]
}

template<typename... Ts>
void bar (std::tuple<Ts...> targs) {
	size_t sz = sizeof...(Ts);
	fuPRmsg("zzz nelems=%lu\n", sz); //fu==>   bar(std::tuple<_UTypes ...>) [with Ts = {int, int, int}]
	                                 //prints 3 correctly
}

template<typename T, size_t... Idx>
void foo (void) {
	size_t sz = sizeof...(Idx);
	fuPRmsg("foo !!! nelems=%lu\n", sz); //fu==>
//foo() [with T = long long unsigned int; long long unsigned int ...Idx = {0, 1, 2, 3, 4}]
} //, when invoked as                                              foo<size_t,0,1,2,3,4>();


template<typename T, size_t... Idx>
void qux (std::integer_sequence<size_t, Idx...> intseq) {
	size_t sz = sizeof...(Idx);
	fuPRmsg("qux nelems=%lu\n", sz); //fu==>
//qux(std::integer_sequence<long long unsigned int, Idx ...>) [with T = long long unsigned int; long long unsigned int ...Idx = {0, 1, 2, 3, 4, 5, 6}]
}



template<typename... Ts>
constexpr auto get_type_sizes() {
	fuPRmsg("sizeof...(Ts) %lu\n", sizeof...(Ts));
	return std::array<std::size_t, sizeof...(Ts)>{sizeof(Ts)...};
}    // Lists sizeof(Ti) for each arg's type Ti:  ^^^^^^^^^^^^^



#if VER_ge17
template<typename T1, typename... Ts>     //// fold expr used to *impl* a variadic fu te
constexpr bool areAllArgTypesSame (T1, Ts...) {
	fuPRmsg("|Ts| = %lu\n", sizeof...(Ts));
	return ( std::is_same_v<T1,Ts> && ... );
} //                               ^^____________that's the binary op.
#endif

void test__variadicFuTe_implemented_with_foldExpr () {
	PRenteredFU;
#if VER_ge17
	bool b0 = areAllArgTypesSame("bunnies",  "");
	bool b1 = areAllArgTypesSame("bunnies",  3.14f);
	bool b2 = areAllArgTypesSame("bunnies",  987654321LU);
	assert(b0 && !b1 && !b2);
#endif
}


struct Xa {
	void exec () { puts("Xa"); }
};
struct Xb {
	void fork () { puts("Xb"); }
};
template<typename... Bases>
struct Xc : public Bases ... {
	void fsck () { puts("Xc"); }
};
struct Xd : public Xa,Xb {         // "public Xa,Xb" here equiv to Xc's  "public Bases..."
	void fsck () { puts("Xd"); }
};
void test__bases () {
	PRenteredFU;
	Xc<Xa,Xb> xcee;
	xcee.fork();
	Xd xdee;
	xdee.fork();
}


/* Variadic cla te, also using recursion.   ...?????
*/
template<typename T, typename... Ts>     ///1ry
struct SortaTuple {
	T                  _elem_car;
	SortaTuple<Ts...>  _rest;
	//
	SortaTuple (T const& elem_car, Ts const&... rest) : _elem_car(elem_car), _rest(rest...) {
		PRtyp(_elem_car);
		PRtyp(_rest);
		fuPRmsg("Cstructed 1ry; |rest|=%lu\n", sizeof...(rest));
	}
	constexpr int size() const { return 1 + _rest.size(); }
};

template<typename T>                      /// partial specializat
struct SortaTuple<T> {
	T                   _elem_theOnly;
	//
	SortaTuple (T const& elem_theOnly) : _elem_theOnly(elem_theOnly) {
		PRtyp(_elem_theOnly);
		fuPRlit("Cstructed base-case");
	}
	constexpr int size() const { return 1; }
};


int main () {

	SortaTuple<float,unsigned,long> sorta__f_u_l{7.7f, 7U, 7L};
	SAYevalPRret(  sorta__f_u_l.size()  ,"%d"  );

	PRtyp( sorta__f_u_l._elem_car ); // float
	PRtyp( sorta__f_u_l._rest._elem_car ); // unsigned
	PRtyp( sorta__f_u_l._rest._rest._elem_theOnly ); // long
	long sevenL = sorta__f_u_l._rest._rest._elem_theOnly;

	

	auto sizes = get_type_sizes<char, int, long long>(); // expect 1+4+8=13
	PRtyp(sizes);
	SAYevalCHKret((std::accumulate(sizes.cbegin(), sizes.cend(), 0LLU)) ,"%llu",13LLU);

	test__fsum();


	qoo<std::make_index_sequence<5>>();

	foo<size_t,0,1,2,3,4>();
//	foo<std::make_index_sequence<5>>();   //No.

	qux<size_t,0,1,2,3,4>(std::make_index_sequence<5>{});
	qux<size_t>(std::make_index_sequence<5>{});           // Same effect; good!

	std::tuple<float,unsigned,long> t3i = std::tuple(3.3f,55U,77L);

//	countDown__b<decltype(t3i), std::tuple_size<decltype(t3i)>::value>(t3i);

	zoo(t3i);
	bar(t3i);

//	zoo<std::make_index_sequence<7>>();
//	foo<std::make_index_sequence<7>>(std::make_index_sequence<7>{});

	test__prSizeof();
	test__prVal_seq_elems_byIndex();
	test__variadicFuTe_implemented_with_foldExpr();
	test__bases();
}
