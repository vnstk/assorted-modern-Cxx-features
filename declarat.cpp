/*   (c)2024 Vainstein K.   */
#include "common.h"
#include <utility> // For std::pair
#include <tuple>
#include <string>
#include <optional>
#include <variant>
#include <vector>
#include <any>
#include <time.h> // For clock()

struct Foo {
	float     _f;
	uint8_t   _hhu;
};

struct Bar {
	long _i;
	float _f;
	Bar(std::tuple<long  ,float> tu) : _i(std::get<0>(tu)), _f(std::get<1>(tu)) {
		printf("(Ln%d) cstructed a Bar from a <long,float> pair; %ld/%.2f\n",__LINE__,_i,_f); }
	Bar(           long i,float f)   : _i(i),         _f(f)         {
		printf("(Ln%d) cstructed a Bar from a long and a float; %ld/%.2f\n",__LINE__,_i,_f); }
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

#if VER_ge14
// Just "auto" conveys the underly type ("int" here), but fails to convey the refness ("int&").
         auto  f00 (int& x) { return x; }
//
// But "decltyp(auto)" conveys the refness too!
decltype(auto) f01 (int& x) { return x; }
#endif


          float g00 (void) { return 3.5; }
constexpr float g01 (void) { return 3.5; }
#if VER_ge20
consteval float g02 (void) { return 3.5; }
consteval int gi03 (int x) { return x * 2; }
#endif

float gretU = g00();
float gretV = g01();
#if VER_ge20
float gretW = g02();

int i_0{42};
const int i_1{42};
constexpr int i_2{42};

//int giretX = gi03(i_0);        //Err, because "i_0" is not constant.
int giretY = gi03(i_1);
int giretZ = gi03(i_2);
#endif


/*
******************************* Structured Binding
*/
void test__structured_binding ()
{	PRenteredFU;
#if VER_ge17
	//------ bindg an array -------
	long arr[3] = {101L, 280L, 237L};
	auto        [ar0, ar1, ar2] = arr; // BTW, this performs a copy --- it's not just innocent aliasing.
	const auto  [c_ar0, c_ar1, c_ar2] = arr; // BTW, this performs a copy --- it's not just innocent aliasing.
	auto&       [refLv_ar0, refLv_ar1, refLv_ar2] = arr; // no copying?
	const auto& [c_refLv_ar0, c_refLv_ar1, c_refLv_ar2] = arr; // no copying?
	auto&&      [refRv_ar0, refRv_ar1, refRv_ar2] = arr; // no copying?? no clue!
	//
	static_assert(std::is_same<      long , decltype(        ar0)>());
	static_assert(std::is_same<const long , decltype(      c_ar0)>());
	static_assert(std::is_same<      long , decltype(  refLv_ar0)>());
	static_assert(std::is_same<const long , decltype(c_refLv_ar0)>()); // Here, constness is carried across
	static_assert(std::is_same<      long , decltype(  refRv_ar0)>());

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

	//------ bindg a class obj --------
	Qux obj{42L, 33U};
	auto        [ob0, ob1, ob2, ob3] = obj;
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


int main ()
{
	test__structured_binding();
	test__piecewise_construct();

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


	Foo x0 = {3.14f, 0xFA};

#if VER_ge11
	Foo x1{   3.14f, 0xFA};
#endif

#if VER_ge20 // Newer-fangled: must still be in decl order.
	Foo x2{ ._f = 3.14f , ._hhu = 0xFA };
	Foo x3={ ._f = 3.14f , ._hhu = 0xFA };
	Foo x4{ ._f{3.14f} , ._hhu{0xFA} };
	Foo x5={ ._f{3.14f} , ._hhu{0xFA} };
#endif

#if VER_ge17
	// std::in_place, use with optional
	//
    // calls std::string( initializer_list<CharT> ) constructor
    std::optional<std::string> o4(std::in_place, {'a', 'b', 'c'});
	printf("(Ln%d)  o4=\"%s\"\n",__LINE__,o4->c_str());
	//
    // calls std::string( size_type count, CharT ch ) constructor
    std::optional<std::string> o5(std::in_place, 3, 'A');
	printf("(Ln%d)  o5=\"%s\"\n",__LINE__,o5->c_str());
	//
//	std::optional<std::string> o3(               3, 'A');  //Err!!  Is no optional:: ctor taking int + char.
		//
		// Similarly, variant; NB, here we need "in_place_type", to specify *which* of the poss types we want.
    std::variant<std::string,float> v0(std::in_place_type<std::string>, 3, 'A');
		// Similarly, any
    std::any                        a0(std::in_place_type<std::string>, 3, 'A');

	// With std::variant, can alternatively specify which type desired, with in_place_index.
    std::variant<float,std::vector<bool>,std::string,unsigned> v1(std::in_place_index<2>, 33, 'Z');
	// In list of poss types, std::string is at offset 2.  (I.e., is 3rd.)
	printf("(Ln%d)  v1=\"%s\"\n",__LINE__,std::get<2>(v1).c_str());
#endif

}
