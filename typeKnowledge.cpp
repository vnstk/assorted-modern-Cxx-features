/*   (c)2024 Vainstein K.   */
#include "common.h"
#include <string.h>
#include <string>
#include <vector>
#include <stdlib.h>

#if VER_ge17
#	define VOID_t std::void_t
#else
template< class... >
using VOID_t = void;
#endif

float somefu (void) { return 3.14159F; }


/* Lessons re __cxa_demangle(typeinfo.name()) approach:
	o	gives core class name (e.g. "Foo") correctly
	o	also recognizes constness of objs and ptrs; array-ness; array bounds.
	o	handed an instance member meth, qualifies with class name and even const-ness, e.g.
			int (Foo::*)(float,char) const
	o	handed a static member meth, does not qualify with class name; e.g.,
			float (bool)
	o	And that's it.  Basically, it'd be great for C coding.
Foo const* [2]
*/



//These could surely pertain only to a type, not to an obj:
//		, std::is_polymorphic<decltype(z)>::value ? " polymorphic" : ""   \
//		, std::is_abstract<decltype(z)>::value ? " abstract" : ""   \

class Foo {
	const int _x;
	int _y;
public:
	Foo (int x) : _x(x), _y(42) { }
	constexpr int bar (void) const { return _x; }
	constexpr int qux (void)       { return _y; }
	static float obtainPiMaybe (bool wantPi) { return wantPi ? 3.14159F : 0.0F; }
	Foo ()=delete;
};

class ZooA {
public:
	int _somenum{111};
	int qux (void)       { return _somenum; }
};
//
class ZooB {
public:
	int _somenum{111};
	int qux (void)       { return _somenum; }
	ZooB (int x) : _somenum(x) {}
};
//
class ZooC {
public:
	int _somenum{111};
	int qux (void)       { return _somenum; }
	ZooC () =delete;
};


void test__declval()
{	PRenteredFU;
	decltype(ZooA().qux()) quxret1 = 44; // no ctor defined, no ctor deleted, fine
#if 0
	decltype(ZooB().qux()) quxret2 = 44; // non-default ctor defined, hence no can do!
	decltype(ZooC().qux()) quxret3 = 44; // default ctor deleted, hence no can do!
#endif
	decltype(std::declval<ZooB>().qux()) quxret2 = 44; // workaround, with declval.
	decltype(std::declval<ZooC>().qux()) quxret3 = 44; // workaround, with declval.
}


/* How to make your own ty trai; 1ry and then full specializats.
*/
template <typename T>
struct is_floatingPt              { static const bool value = false; };
//
template <>
struct is_floatingPt<float>       { static const bool value = true; };
//
template <>
struct is_floatingPt<double>      { static const bool value = true; };
//
template <>
struct is_floatingPt<long double> { static const bool value = true; };

// Not strictly necessary, but allows more concise **and hence spiffier** code.
template <typename T>
inline constexpr bool is_floatingPt_v = is_floatingPt<T>::value;

// But wait!!!  We can achieve yet *****higher***** degree of spiffiness!
template <typename T>
constexpr const char* is_floatingPt_huh = is_floatingPt<T>::value ? "YES" : "NO";

void test__myOwn_ty_trai ()
{	PRenteredFU;
	float x;
	long  y;

	fuPRmsg("x is_floatingPt: %s\n", is_floatingPt<decltype(x)>::value ? "YES" : "NO");
	fuPRmsg("y is_floatingPt: %s\n", is_floatingPt<decltype(y)>::value ? "YES" : "NO");

	fuPRmsg("x is_floatingPt: %s\n", is_floatingPt_v<decltype(x)> ? "YES" : "NO");
	fuPRmsg("y is_floatingPt: %s\n", is_floatingPt_v<decltype(y)> ? "YES" : "NO");

	fuPRmsg("x is_floatingPt: %s\n", is_floatingPt_huh<decltype(x)>);
	fuPRmsg("y is_floatingPt: %s\n", is_floatingPt_huh<decltype(y)>);

	static_assert(  is_floatingPt_v<float>);
	static_assert(! is_floatingPt_v<long>);
}


/* See whether given has a particular members; to be safe, check member *kind* as well as name.
   We'll have to create a compiletime *something* to hold answ to each such question.
   Also: in addition to the "whether", we'll want to print (via typeid) type of such a memb if
   		found; so, perhaps, ret NULL if not found and mangled type name if found??
*/

template <typename, typename = VOID_t<>>
struct has_integral_memberType__size_type : std::false_type {};
/*
**     This specialization doesn't check that size_type is integral!
**
template <typename T>
struct has_integral_memberType__size_type<T
	, std::void_t<typename T::size_type>
> : std::true_type {};
*/
template <typename T>
struct has_integral_memberType__size_type<T
	, typename std::enable_if<std::is_integral<typename T::size_type>::value>::type
> : std::true_type {};

class Flarp { using size_type = std::string; };

void test__detecting_members ()
{	PRenteredFU;
	PRmsg("has a proper size_type, float?  %s\n", has_integral_memberType__size_type<float>::value ?"Yes":"No");
	PRmsg("has a proper size_type, Foo?  %s\n", has_integral_memberType__size_type<Foo>::value ?"Yes":"No");
	PRmsg("has a proper size_type, Flarp?  %s\n", has_integral_memberType__size_type<Flarp>::value ?"Yes":"No");
	PRmsg("has a proper size_type, std::vector<float>?  %s\n", has_integral_memberType__size_type<std::vector<float>>::value ?"Yes":"No");
}
/*
template <typename T, typename T_MemberType>
const char* nameOf_member__type ();

template <typename T, ???>
bool has_member__static_fu ();

template <typename T, ???>
bool has_member__inst_meth ();

template <typename T, typename T_MemberAliasTe>
bool has_member__alias_te ();
*/
// Other kinds of members: static datamemb, inst datamemb, typedefs, usings (non-te aliases???), enumerations, nested classes.
// ---see https://en.cppreference.com/w/cpp/language/class#Member_specification


using arrf42_t = std::array<float,42>;
//
void test__ways_to_learnT_of_arrayTN ()
{	PRenteredFU;
#if VER_ge17
	std::tuple_element<41,arrf42_t>::type       f01 = 1.111F;
///	std::tuple_element<42,arrf42_t>::type f02 = 2.2F;  //ERR!  Cpiletime out-of-bounds
	PRnameTYPmangled(f01); // Should print "f"

	arrf42_t arrf42Inst;
	std::decay<decltype(arrf42Inst[0])>::type   f03 = 3.333F;
	PRnameTYPmangled(f03); // Should print "f"
#endif
}


template<typename T>
const char *figureSignedness (T targ) {
	if (std::is_integral<T>()) {
		if (std::is_signed<T>()) return "signed";
		else if (std::is_unsigned<T>()) return "unsigned";
		else return "?? integral but neither signed not unsi";
	}
	return "N/A";
}
//
void test__checkSignedness ()
{	PRenteredFU;
	PRmsg("true     --- %s\n", figureSignedness(true));
	PRmsg("42       --- %s\n", figureSignedness(42));
	PRmsg("42U      --- %s\n", figureSignedness(42U));
	PRmsg("3.57     --- %s\n", figureSignedness(3.57));
	PRmsg("func ptr --- %s\n", figureSignedness(figureSignedness<unsigned long>));
	const unsigned cu = 42U;
	PRmsg("cu      --- %s\n", figureSignedness(cu));
}


void test__reckonArithType ()
{	PRenteredFU;
	bool b = false;
	int i = 42;
	const int ci = 42;
	unsigned int u = 42U;
	unsigned int& ru = u;
	const unsigned cu = 42U;
	const unsigned& rcu = cu;
	int& ri = i;
	const int& rci = ci;
	long l = 42L;
	unsigned long lu = 42LU;
	float f = 1.1F;
	long double ld = 1.1;
	const long double cld = 1.1;

	PRarithType(b);
	PRarithType(i);
	PRarithType(ci);
	PRarithType(ri);
	PRarithType(rci);

	PRarithType(u);
	PRarithType(ru);
	PRarithType(cu);
	PRarithType(rcu);

	PRarithType(l);
	PRarithType(lu);

	PRarithType(f);
	PRarithType(ld);
	PRarithType(cld);
}


int main()
{
	test__reckonArithType();
	test__checkSignedness();

	test__ways_to_learnT_of_arrayTN();

	test__detecting_members();
	test__myOwn_ty_trai();

	float pi0 = 3.14159F;
	const float pi1 = 3.14159F;

	const Foo fooa{77};
	Foo       foob{77};

	Foo& x0 = foob;
	const Foo& x1 = fooa;
	const Foo& x2 = foob;
	Foo *p0 = &foob;
	Foo *const p1 = &foob;
	const Foo *p2 = &fooa;
	const Foo *const p3 = &fooa;

	Foo fooarr[] = {Foo(55), Foo(66), Foo(77)};
	const Foo* pfooarr[] = {p2, p3};

	PRnameTYPmangled(x0);
	PRnameTYPmangled(x1);
	PRnameTYPmangled(x2);
	PRnameTYPmangled(p0);
	PRnameTYPmangled(p1);
	PRnameTYPmangled(p2);
	PRnameTYPmangled(p3);

	PRtyp(x0);
	PRtyp(x1);
	PRtyp(x2);
	PRtyp(p0);
	PRtyp(p1);
	PRtyp(p2);
	PRtyp(p3);
	//
//	PRtyp(ty_tra_describe<int>);
//	PRtyp(Foo::obtainPiMaybe);
	PRtyp(Foo::bar);
	PRtyp(fooarr);
	PRtyp(pfooarr);

	PRwhat(fooa);
	PRwhat(foob);
	PRwhat(x0);
	PRwhat(x1);
	PRwhat(x2);
	PRwhat(p0);
	PRwhat(p1);
	PRwhat(p2);
	PRwhat(p3);

	PRwhat(fooarr);
	PRwhat(pfooarr);
//	PRwhat(Foo::obtainPiMaybe);
	PRwhat(Foo::bar);
	PRwhat(Foo::qux);
	PRwhat(fooa);
	PRwhat(foob);
	PRwhat(x0);
	PRwhat(x1);
	PRwhat(x2);
	PRwhat(p0);
	PRwhat(p1);
	PRwhat(p2);
	PRwhat(p3);
//	PRwhat(ty_tra_describe<int>);
//	PRwhat(somefu);
	PRwhat(pi0);
	PRwhat(pi1);

	auto foolambda = [](Foo&& foorval) {
		PRwhat(foorval);
	};
	foolambda(Foo(88));
	PRwhat(foolambda);

//	PRwhat(  [](Foo&& foorval){} );

	decltype(pi0) pi2 = 3.14;
	decltype(pi0)* ppi2 = &pi2;

	test__declval();
}
