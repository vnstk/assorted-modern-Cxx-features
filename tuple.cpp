/*   (c)2024 Vainstein K.   */
#include "common.h"
#include <tuple>


struct Qux {
	long _i;
	float _f;
	const char *_cs;
	Qux(long i, float f, const char *cs): _i(i), _f(f), _cs(cs) {
		printf("(Ln%d) cstructed a Qux, _i=%ld _f=%.3f _cs='%s'\n",__LINE__,_i,_f,_cs);
	}
};

std::tuple<bool,double,unsigned> mkTu (unsigned u) {
#if VER_ge17
	return                {true, 3.3, u};
#else
	return std::make_tuple(true, 3.3, u);
#endif
}

uint8_t takesTu (std::tuple<long,float,const char*>&& tu) {
	return (uint8_t)
	printf("(Ln%d)  /%s/  called with <%ld,%.2f,%s>\n",
	       __LINE__, currFU, std::get<0>(tu), std::get<1>(tu), std::get<2>(tu));
}

uint8_t takesDiscrete (long i, float f, const char* cs) {
	return (uint8_t)
	printf("(Ln%d)  /%s/  called with %ld,%.2f,%s\n", __LINE__, currFU, i,f,cs);
}

unsigned mulThreeUints (unsigned u1, unsigned u2, unsigned u3) {
	return u1*u2*u3;
}


// Adapted from examp at https://en.cppreference.com/w/cpp/utility/tuple/tuple_element
//
template<typename TUP, std::size_t Idx = 0>
void dumpTupElemTypes (TUP tup) {
#if VER_ge17
	if constexpr (Idx < std::tuple_size<TUP>::value) {
		using itemType = typename std::remove_reference<std::tuple_element_t<Idx,TUP>>::type;
		itemType tmpelem;
		PRtyp(tmpelem);
		//
		dumpTupElemTypes<TUP, Idx + 1>(tup);
	}
#endif
}


void test__making_a_tuple__and__std_tie()
{	PRenteredFU;

	// make a tuple, one way
	std::tuple<long,float,const char*> t0 = std::make_tuple(42L,4.2f,"bunnies");
	auto                               t1 = std::make_tuple(42L,4.2f,"bunnies");
	static_assert(std::is_same<decltype(t0), decltype(t1)>());
#if VER_ge17
	static_assert(std::is_same_v<decltype(t0), decltype(t1)>);
#endif

	// make a tuple, anoth way
	auto t4 = std::tuple<long,float,const char*>(42L,4.2f,"bunnies");

#if VER_ge17
	// make a tuple, yet anoth way
	std::tuple<long,float,const char*> t5 = {42L,4.2f,"bunnies"};
#endif

	// make a tuple *of rval refs to*
	std::tuple<long&&,float&&,const char*&&> t6 = std::forward_as_tuple(42L,4.2f,"bunnies");


	// std::tie() creates a tuple of lvalue references to its arguments, or to insts of std::ignore.
	long t0i; float t0f; const char *t0cs;
	std::tie(t0i,    t0f,     t0cs) = t0; // Unpacking a tuple, all its fields.
	//
	long t1i;            const char *t1cs;
	std::tie(t1i, std::ignore, t1cs) = t1; // Unpacking selected elems of a tuple
	//            ^^^^^^^^^^^
	//
	auto t3 = std::tie(t0i,t0f,t0cs); // Paradoxically, std::tie() can also *create* a tuple!
	PRwhat( std::get<0>(t3) );
	static_assert(std::is_same<long& , decltype(std::get<0>(t3))>());
	// Although "t0i" was "long", corresp tuple elem is "long&" --- refness is not preserved.
}


void test__structured_binding()
{	PRenteredFU;

	float struBindi_x;
	char  struBindi_y;
	int   struBindi_z;
	std::tuple<float&, char&&, int> t7(struBindi_x, std::move(struBindi_y), struBindi_z);
	std::tuple<float&, char&&, int> t2(struBindi_x, std::move(struBindi_y), struBindi_z);
	//
	// structured binding: an alternative to std::tie, for unpacking a tuple:
	// (Big attraction is preserving refness, not just the underly type.)
#if VER_ge17
	// Curious subtleties, dependg on constness and refness of the array-of-binders itself:

//	auto [aaBinder, bbBinder, ccBinder] = t7;  //Err: cannot unpack an rvalue into non-ref binder.

	auto&       [aaBinder, bbBinder, ccBinder] = t7;
	static_assert(std::is_same<float&    , decltype(aaBinder)>());
	static_assert(std::is_same<char&&    , decltype(bbBinder)>());
	static_assert(std::is_same<      int , decltype(ccBinder)>());

	const auto& [aBinder, bBinder, cBinder] = t2;
	static_assert(std::is_same<float&    , decltype(aBinder)>());
	static_assert(std::is_same<char&&    , decltype(bBinder)>());
	static_assert(std::is_same<const int , decltype(cBinder)>()); // Note, now *const* int!
	//                         ^^^^^
#endif
}


void test__retrieving_a_particular_elem()
{	PRenteredFU;
	long t0i; float t0f; const char *t0cs;
	auto t3 = std::tie(t0i,t0f,t0cs);

	// Retrieving a particular elem, std::get
	auto getByIndex = std::get<0>(t3);
	static_assert(std::is_same<long , decltype(getByIndex)>());

	// But wait!!  Also can std::get by elem type!  (Splendid foot-shooting opp'ty.)
#if VER_ge14
	auto getByTypeA = std::get<long&>(t3);                      //Implicitly casts int to long
	static_assert(std::is_same<long , decltype(getByTypeA)>()); //Refness went away, per decltype...
//	const auto getByTypeB = std::get<long >(t3);          //Err// ...but not according to std::get !
	const auto getByTypeB = std::get<long&>(t3);

	static_assert(std::is_same<const long , decltype(getByTypeB)>());
	//                         ^^^^^
#endif
}


void test__tuple_cat__and__common_type()
{	PRenteredFU;

	int oneNum{42};
	// Single-elem tuple,
	auto tsingle = std::tie(oneNum);

	std::tuple<long,float,const char*> t0 = std::make_tuple(42L,4.2f,"bunnies");
	// catenate tuples, tuple_cat
	auto tmany = std::tuple_cat(t0, tsingle, mkTu(12345U));
	//cpiletime knowledge
	static_assert(7 == std::tuple_size<decltype(tmany)>::value);
#if VER_ge17
	static_assert(7 == std::tuple_size_v<decltype(tmany)>);
#endif
	// Won't work, because "i" is not constexpr, and std::get<N> wants constexpr N.
	   /*
	for (size_t i = 0U; i < std::tuple_size<decltype(tmany)>::value; ++i) {
		printf("%lu ==>...  %s\n", i, fundTypName(std::get<i>(tmany)));
	}  */
	dumpTupElemTypes(tmany);

	std::tuple_element<5,decltype(tmany)>::type flarp;
	PRwhat(flarp);
	PRaboutTYP(flarp);
	static_assert(std::is_same<double, decltype(flarp)>());

	auto t8 = mkTu(777U);
#if VER_ge23
	//Whaaaa?? TODO: come back to, once GCC supports this
	std::common_type<decltype(t4),decltype(t8)>::type fuse4and9;
	PRwhat(fuse4and9);
#endif
}


void test__apply()
{	PRenteredFU;
	auto t4 = std::tuple<long,float,const char*>(42L,4.2f,"bunnies");
	auto                               t1 = std::make_tuple(42L,4.2f,"bunnies");

	uint8_t retplain = takesTu(std::move(t4));
#if VER_ge17
	// Call some func, with args coming from elems of a tuple.
	uint8_t retapply = std::apply(takesDiscrete, t1);
#endif
#if VER_ge23
	// std::array and std::pair are "honorary tuples"; can do tupley things with them.
	std::array<unsigned,3> arrThreeUints{7U, 11U, 13U};
	const unsigned oneThouAndOne = std::apply(mulThreeUints, arrThreeUints);
	static_assert(3 == std::tuple_size_v<decltype(arrThreeUints)>);
	printf("Behold, %u nights!\n", oneThouAndOne);
#endif

	long t0i; float t0f; const char *t0cs;
	Qux qA{ t0i, t0f, t0cs };
#if VER_ge17
	// Call some class's ctor with args coming from elems of a tuple.
		// Same as std::apply really, but the target fu is a ctor.
	auto qB = std::make_from_tuple<Qux>(t1);
	static_assert(std::is_same_v<decltype(qA), decltype(qB)>);
#endif
}


int main () {
	test__making_a_tuple__and__std_tie();
	test__structured_binding();
	test__retrieving_a_particular_elem();
	test__tuple_cat__and__common_type();
	test__apply();
}
