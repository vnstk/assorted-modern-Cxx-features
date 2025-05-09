/*   (c)2024 Vainstein K.   */
#include "common.h"
#include <utility> // For std::pair
#include <map>
#include <tuple>
#include <string>
#include <optional>
#include <variant>
#include <vector>
#include <any>
#include <time.h> // For clock()

struct Bar {
	long _i;
	float _f;
	Bar(std::tuple<long  ,float> tu) : _i(std::get<0>(tu)), _f(std::get<1>(tu)) {
		fuPRmsg("cstructed a Bar from a <long,float> pair; %ld/%.2f\n",_i,_f); }
	Bar(           long i,float f)   : _i(i),         _f(f)         {
		fuPRmsg("cstructed a Bar from a long and a float; %ld/%.2f\n",_i,_f); }
};

struct Qux {
	long            _i;
	long&           _ri;
	const unsigned  _cu;
	const unsigned& _cru;
	Qux (long i, unsigned u) : _i(i), _ri(_i), _cu(u), _cru(_cu){ printf("cstructed a Qux\n"); }
};


float conjure_a_float (void) {
	printf("Fu  %s  invoked.\n", currFU);
	return (float) clock();
}
float conjure_a_float__not_def (void);


          float g00 (void) { return 3.5; }
constexpr float g01 (void) { return 3.5; }
#if VER_ge20
consteval float g02 (void) { return 3.5; }
consteval int gi03 (int x) { return x * 2; }
#endif



void test__decltype_and_declval()
{	PRenteredFU;

	int iarg{42};
	const int jarg{42};
	constexpr int karg{42};

	float gretA = g00();
	float gretB = g01();
#if VER_ge20
	float gretC = g02();
//	int giretXi = gi03(iarg);        // Err, because iarg is not cpiletime-const.
	int giretXj = gi03(jarg);
	int giretXk = gi03(karg);
#endif

	const float gretD = g00();
	const float gretE = g01();
#if VER_ge20
	const float gretF = g02();
#endif

//	constexpr float gretP = g00();    //Err, because g00 can't run at cpiletime.
	constexpr float gretQ = g01();
#if VER_ge20
	constexpr float gretR = g02();
#endif


	float                          fffA; // Old-fashioned
#if VER_ge20
	decltype( []{return 3.14f;} )  fffB; // New-fangled
#endif
	// Less silly:
	decltype(conjure_a_float)      ffC; // Note, conjure_a_float() was not actually invoked.

	decltype(conjure_a_float__not_def) ffD; // Not even defined!

	int     xval{42};
	int& refxval = xval;
#if 0 //TODO !
#if VER_ge14
//	auto f00_ret = f00(refxval);
	static_assert(std::is_same<int , decltype(f00)>());
//	static_assert(std::is_same<int , decltype(f00_ret)>());
	//
//	auto f01_ret = f01(refxval);
//	PRwhat(f01_ret);
//	PRtyp(f01_ret);
	static_assert(std::is_same<int&, decltype(f01)>());
#endif
#endif
}


#if VER_ge14
// Just "auto" conveys the underly type ("int" here), but fails to convey the refness ("int&").
         auto  f00 (int const& x) { return x; }
//
// But "decltyp(auto)" conveys the refness too!
decltype(auto) f01 (int const& x) { return x; }

//Similarly,
template<typename T>
         auto  g00 (T (&arr) [30]) { return arr; }

template<typename T>
decltype(auto) g01 (T (&arr) [30]) { return arr; }

#endif

struct Fubar {
};

void test__decltype_subtleties()
{	PRenteredFU;

	Fubar fb;
	decltype(fb)   notParenthesized;
	PRwhat(notParenthesized); // "class" ==> what we expected.
	decltype((fb)) yesParenthesized = fb;
	PRwhat(yesParenthesized); // "lvalue ref" ==> Nope!   So, don't parenthesize.

#if VER_ge14
	PRtyp(f00);        // ret type reported as    int
	PRtyp(f01);        // ret type reported as    int const&
	PRtyp(g00<float>); // ret type reported as    float*
	PRtyp(g01<float>); // ret type reported as    float[30]
#endif

}



struct Foo {
    char _ch;
    Foo () {   fuPRmsg("dflt ctor; _ch=%c\n",_ch);   }
    Foo (char ch) : _ch(ch) {   fuPRmsg("_ch=%c\n",_ch);   }
    Foo (Foo const& rhs) : _ch(rhs._ch) {   fuPRmsg("_ch=%c\n",_ch);   }
	Foo& operator= (Foo const& rhs) { if (&rhs != this) { _ch = rhs._ch; } return *this; }
    Foo (Foo&& rhs) : _ch(std::move(rhs._ch)) {   fuPRmsg("_ch=%c\n",_ch);   }
	Foo& operator= (Foo&& rhs) { if (&rhs != this) { _ch = rhs._ch; rhs._ch = '\0'; } return *this; }
};


void test__structured_binding ()
{	PRenteredFU;
#if VER_ge17
	//------ bindg an array -------
	{
		long arr[3] = {101L, 280L, 237L}; // "arr longa, vita brevis est"????
		auto        [ar0, ar1, ar2] = arr; // BTW, this performs a copy --- it's not just innocent aliasing.
		auto const  [c_ar0, c_ar1, c_ar2] = arr; // BTW, this performs a copy --- it's not just innocent aliasing.
		auto&       [refLv_ar0, refLv_ar1, refLv_ar2] = arr; // no copying?
		auto const& [c_refLv_ar0, c_refLv_ar1, c_refLv_ar2] = arr; // no copying?
		auto&&      [refRv_ar0, refRv_ar1, refRv_ar2] = arr; // no copying?? no clue!
		PRwhat(        ar0);
		PRwhat(      c_ar0);
		PRwhat(  refLv_ar0);
		PRwhat(c_refLv_ar0);
		PRwhat(  refRv_ar0);
		static_assert(std::is_same<long       , decltype(        ar0)>());
		static_assert(std::is_same<long const , decltype(      c_ar0)>());
		static_assert(std::is_same<long       , decltype(  refLv_ar0)>());
		static_assert(std::is_same<long const , decltype(c_refLv_ar0)>()); // Here, constness is carried across
		static_assert(std::is_same<long       , decltype(  refRv_ar0)>());
		SAYeval(    refLv_ar0 = 33L  );
		PRmsg("Now arr[0] = %ld\n", arr[0]);
#if 0                                           // Err: assignm of rdonly
		SAYeval(  c_refLv_ar0 = 55L  );
#endif
		SAYeval(    refRv_ar0 = 77L  );
		PRmsg("Now arr[0] = %ld\n", arr[0]);
	}

	// Let's make sure about that "no copying", eh?
	{
		Foo arr[3] = {{'X'}, {'Y'}, {'Z'}};
		//
		printf("\n/%d/  Shall decl-and-init array of type \"auto\".\n", __LINE__);
		auto        [ar0, ar1, ar2] = arr; // Yes, entails copying.
		//
		printf("\n/%d/  Shall decl-and-init array of type \"auto const\".\n", __LINE__);
		auto const  [c_ar0, c_ar1, c_ar2] = arr; // Yes, entails copying.
		//
		printf("\n/%d/  Shall decl-and-init array of type \"auto&\".\n", __LINE__);
		auto&       [refLv_ar0, refLv_ar1, refLv_ar2] = arr; // Indeed, no copying!
		//
		printf("\n/%d/  Shall decl-and-init array of type \"auto const&\".\n", __LINE__);
		auto const& [c_refLv_ar0, c_refLv_ar1, c_refLv_ar2] = arr; // Indeed, no copying!
		//
		printf("\n/%d/  Shall decl-and-init array of type \"auto&&\".\n", __LINE__);
		auto&&      [refRv_ar0, refRv_ar1, refRv_ar2] = arr; // Indeed, no copying!  (or moving)
		//
		printf("\n/%d/  Shall decl-and-init array of type \"auto&&\", and std::move the RHS.\n", __LINE__);
		const char arr0chBefore = arr[0]._ch;
		auto&&      [refRv_ar0__mv, refRv_ar1__mv, refRv_ar2__mv] = std::move(arr); // Indeed, no copying!  (or moving)
		const char arr0chAfter = arr[0]._ch;
		assert(arr0chBefore == arr0chAfter); // Josuttis says move should've occurred; but it didn't.
		//
		PRwhat(        ar0);
		PRwhat(      c_ar0);
		PRwhat(  refLv_ar0);
		PRwhat(c_refLv_ar0);
		PRwhat(  refRv_ar0);
		PRwhat(  refRv_ar0__mv);
	}
	/* Conclusions:
			o	"auto const [...." and "auto const& [...." do confer constness.
			o	As compared to "auto [....." and "auto const [.....", the other ways obviate copying.
			o	But "auto& [....." and "auto const& [...." and "auto&& [...." do not confer refness at all.
			o	In the cases above, "&" and "&&" applies to the anonymous pseudo-array holding our new locals.
			o	We can modify whatever had been the RHS in stru-bindg assignm, via both "&" and "&&".
	*/

	//------ bindg a tuple --------
	float tuMemb_x; char tuMemb_y; int tuMemb_z;
	std::tuple<float&, char&&, int> tup(tuMemb_x, std::move(tuMemb_y), tuMemb_z);
//	auto        [tu0, tu1, tu2] = tup;                     // Err!  (don't understand why)
//	const auto  [c_tu0, tc_u1, tc_u2] = tup;               // Err!  (don't understand why)
	auto&       [refLv_tu0, refLv_tu1, refLv_tu2] = tup;
	const auto& [c_refLv_tu0, c_refLv_tu1, c_refLv_tu2] = tup;
	auto&&      [refRv_tu0, refRv_tu1, refRv_tu2] = tup;
	//
	// Seems that constness and refness of the binding-identifier array just doesn't matter here?
	static_assert(std::is_same<  float& , decltype(  refLv_tu0)>());
	static_assert(std::is_same<  float& , decltype(c_refLv_tu0)>()); //Here, constness ignored...?
	static_assert(std::is_same<  float& , decltype(  refRv_tu0)>());
	PRwhat(refLv_tu0);
	PRwhat(c_refLv_tu0);
	PRwhat(refRv_tu0);

	//------ bindg a pair: specific case of bindg tuple --------
	std::map<std::string,double> sdmap;
	// Instead of the oh-so-terribly-hard-to-type
	for (auto const& kv : sdmap) {
		auto const& k = kv.first;   auto const& v = kv.second;
		printf("\t %s ==> %.3f \n", k.c_str(), v);
	}
	// , or the equally difficult
	for (auto const& kv : sdmap) {
		printf("\t %s ==> %.3f \n", kv.first.c_str(), kv.second);
	}
	// , we can now
	for (auto const& [k,v] : sdmap) {
		printf("\t %s ==> %.3f \n", k.c_str(), v);
	}

	//------ bindg a class obj --------
	Qux obj{42L, 33U};
	auto        [ob0, ob1, ob2, ob3] = obj;
	auto        [ob0__x, ob1__x, ob2__x, ob3__x] = (obj);
#if 0                                                         // Josuttis says this is fine; compiler disagrees.
	auto        [ob0__y, ob1__y, ob2__y, ob3__y] = {obj};
#endif
	const auto  [c_ob0, c_ob1, c_ob2, c_ob3] = obj;
	auto&       [refLv_ob0, refLv_ob1, refLv_ob2, refLv_ob3] = obj;
	const auto& [c_refLv_ob0, c_refLv_ob1, c_refLv_ob2, c_refLv_ob3] = obj;
	auto&&      [refRv_ob0, refRv_ob1, refRv_ob2, refRv_ob3] = obj;
	//
	// Seems that constness and refness of the binding-identifier array just doesn't matter here, either?
	static_assert(std::is_same<const unsigned& , decltype(        ob3)>());
	static_assert(std::is_same<const unsigned& , decltype(      c_ob3)>());
	static_assert(std::is_same<const unsigned& , decltype(  refLv_ob3)>());
	static_assert(std::is_same<const unsigned& , decltype(c_refLv_ob3)>());
	static_assert(std::is_same<const unsigned& , decltype(  refRv_ob3)>());
#endif
}


// Josuttis, "C++17", pp33-40.
// Why not just give your class a "toTuple() const" method, and then stru-bind the tuple?
void test__structured_binding__of_userdef_typ ()
{	PRenteredFU;
#if VER_ge17
	//TODO, maybe.
#endif
}




void test__piecewise_construct()
{	PRenteredFU;
	//========================= Ctor tags ==================================================

	// std::piecewise_construct
			// has something to do with std::pair having a ctor overload whose 1st arg is
			// of type std::piecewise_construct_t; somehow std::unordered_map<Key,T>::emplace(Args&&...)
			// knows which std::pair ctor overload to call....?????????
	//
	std::tuple<long,float> tuA( 42L,3.14f);
	std::tuple<long,float> tuB(842L,31.4f);
	printf("Without piecewise_construct, each tuple arg will stay itself (i.e. a tuple).\n");
	std::pair<Bar,Bar> pearX(                          tuA,tuB);
	printf("With piecewise_construct, each N-tuple will donate its elems for use as args to a ctor taking N args.\n");
	std::pair<Bar,Bar> pearY(std::piecewise_construct, tuA,tuB);
}


struct Fnord {
	float     _f;
	uint8_t   _hhu;
};

//  see https://en.cppreference.com/w/cpp/language/aggregate_initialization
void test__aggregate_initialization()
{	PRenteredFU;

	Fnord x0 = {3.14f, 0xFA};

#if VER_ge11
	Fnord x1{   3.14f, 0xFA};
#endif

#if VER_ge20 // Newer-fangled: must still be in decl order.
	Fnord x2{ ._f = 3.14f , ._hhu = 0xFA };
	Fnord x3={ ._f = 3.14f , ._hhu = 0xFA };
	Fnord x4{ ._f{3.14f} , ._hhu{0xFA} };
	Fnord x5={ ._f{3.14f} , ._hhu{0xFA} };
#endif
}


void test__list_initialization()
{	PRenteredFU;
	int xa{42};
	static_assert(std::is_same<int, decltype(xa)>());
	auto xb{42}; // Should be now 'init', not 'initializer_list'! ...we hope.
	static_assert(std::is_same<int, decltype(xb)>());

	auto xc = {42,42};
	static_assert(std::is_same<std::initializer_list<int>, decltype(xc)>());
	auto xd = {42};
	static_assert(std::is_same<std::initializer_list<int>, decltype(xd)>());
}


// Non-const static datamemb can now be initialized inside class def.
// And *every* TU including such a header will have to dedicate storage for it.
struct Flarp {
#if VER_ge17
	inline static std::string sa{"XXX"};
#endif
};


int main ()
{
	test__structured_binding();
	test__decltype_and_declval();
	test__decltype_subtleties();
	test__piecewise_construct();
	test__aggregate_initialization();
	test__list_initialization();
}
