/*   (c)2024 Vainstein K.   */
#include "common.h"
#if ! VER_ge17
#	error "Everything in this file needs 17+"
#endif
#include <optional>
#include <variant>
#include <any>
#include <string>
#include <vector>


struct OloadedFuCallOp {
	void operator() (long arg) { printf("OFCO long arg [%ld], sizeof=%zu\n", arg, sizeof(arg)); }
	void operator() (double arg) { printf("OFCO double arg [%.2f], sizeof=%zu\n", arg, sizeof(arg)); }
	void operator() (std::string const& arg) { printf("OFCO std::str arg [%s], sizeof=%zu\n", arg.c_str(), sizeof(arg)); }
#if VER_ge20 // Use of "auto" in param decl only avail in 20+
	void operator() (auto arg) { printf("/%s/--OFCO-auto  some other arg? sizeof=%zu\n", currFU, sizeof(arg)); }
#else // If can't have auto params, then must cover every contingency explicitly.
	void operator() (unsigned arg) { printf("OFCO unsigned arg [%u], sizeof=%zu\n", arg, sizeof(arg)); }
	void operator() (bool arg) { printf("OFCO bool arg [%d], sizeof=%zu\n", arg, sizeof(arg)); }
#endif
};

#if VER_ge20 // Further, 20+ gives "invoke_r" spiffed-up version of visit().
struct OloadedByRetType {
	int operator() (auto arg) { return printf("/%s/--OBRT-ret--int  sizeof=%zu\n", currFU, sizeof(arg)); }
	float operator() (auto arg) { return printf("/%s/--OBRT-ret--float  sizeof=%zu\n", currFU, sizeof(arg)) ? 5.5f : 6.6f; }
};
OloadedByRetType obrt;
#endif

struct NotDefaultConstructible {
	int _i;
	NotDefaultConstructible (int i) : _i(i) {}
	NotDefaultConstructible() =delete;
};


struct Foo {
	Foo (float f, char ch) {}
	Foo (float f) {}
};


std::optional<int> fancyToInt__oneWay (char const *s) {
	int n = atoi(s);
	if (n)
		return n; // Implicit envelopment by std::optional<>: what could *possibly* go wrong.
	return std::nullopt;
}
std::optional<int> fancyToInt__anothWay (char const *s) {
	std::optional<int> ret;
	int n = atoi(s);
	if (n)
		ret = n;
	return ret;
}



void test__std_optional__retrieveManagedValue()
{	PRenteredFU;
	unsigned const input = 77U;
	std::optional<unsigned> ox{input};
	auto x0 = *ox;
	auto x1 = ox.value();
	auto x2 = ox.value_or(88U);
	auto x3 = std::move(ox);
	assert(input==x0);
	assert(input==x1);
	assert(input==x2);
	assert(input==x3);
}


void test__std_optional__checkTenantPresence()
{	PRenteredFU;
	std::optional<int> ox = fancyToInt__oneWay("42");
	if (ox)
		PRmsg("Cverted to num %d\n", *ox);
	else
		PRlit("Not cverted, empty ox");
	PRmsg("Fallback-cverted ox: %d\n", ox.value_or(-999));
	std::optional<int> oy = fancyToInt__oneWay("thirty-five");
	if (oy)
		PRmsg("Cverted to num %d\n", *oy);
	else
		PRlit("Not cverted, empty oy");
	// Another way
	PRmsg("Cverted oy: %c\n", oy.has_value() ? 'Y':'N');
	PRmsg("Fallback-cverted oy: %d\n", oy.value_or(-999));

	std::optional<unsigned> emptyOu_0;
	std::optional<unsigned> emptyOu_1{std::nullopt}; // Same deal
	assert(emptyOu_0 == emptyOu_1); // "they're both empty"

	std::optional<unsigned> willBeEmptied_0{42U};
	std::optional<unsigned> willBeEmptied_1{42U};
	std::optional<unsigned> willBeEmptied_2{42U};

	willBeEmptied_0 = std::nullopt;
	willBeEmptied_1.reset();
	willBeEmptied_2 = {};

	assert(emptyOu_0 == willBeEmptied_0);
	assert(emptyOu_0 == willBeEmptied_1);
	assert(emptyOu_0 == willBeEmptied_2);
}


void test__std_optional__other()
{	PRenteredFU;
	std::optional o10{5.5F}; // of course won't call Foo's monadic ctor
	std::optional<Foo> o11{5.5F}; // will call Foo's monadic ctor

	// What about ctors with 2+ args?
#if 0
	std::optional<Foo> o12{5.5F, 'Z'}; // Err!!
#endif
	std::optional<Foo> o12{std::in_place, 5.5F, 'Z'}; // calls dyadic ctor, OK

	std::optional<Foo> o13{Foo{5.5F, 'Z'}}; // But this way, create a temporary

	std::optional<Foo> o14 = std::make_optional<Foo>(5.5F, 'Z');

	// anoth examp
    // calls std::string( size_type count, CharT ch ) constructor
    std::optional<std::string> o33(std::in_place, 3, 'A');

	// ctor takg initializer_list ?
	std::optional<std::string> o1(std::in_place, {'a', 'b', 'c'});
	assert(*o1 == "abc");
	std::optional<std::string> o2(std::in_place, 5, 'Z');
	std::string s0{"bunnies"};
	std::optional<std::string> o3(std::move(s0));
	const std::string s1{"bunnies"};
	std::optional<std::string> o4(          s1 );

	o4.emplace(17, 'Q'); // replace with std::string cstructed from these args, fwded.

	// YA way to cstruct an obj && wrap it in an optional,
	std::optional<std::string> o5 = std::make_optional<std::string>(11,'J');
	auto                       o6 = std::make_optional<std::string>(11,'J');

	PRmsg("ctains a value?  %s\n", (bool)o1?"yea":"nay");
	PRmsg("ctains a value?  %s\n", o1.has_value()?"yea":"nay");
	PRmsg("ctains a value==>  \"%s\"\n", o1.value_or("alternVal").c_str());
	o1.reset();
	PRmsg("still ctains a value?  %s\n", (bool)o1?"yea":"nay");//Should't.
	PRmsg("ctains a value==>  \"%s\"\n", o1.value_or("alternVal").c_str());

	PRmsg("o2 ctains \"%s\"\n",  o2->c_str());
	PRmsg("o2 ctains \"%s\"\n",(*o2).c_str());

	//Uninit optional has special type nullopt_t...
	// ...except it doesn't when I tried.  (Both ways.)
	std::optional<std::string> o7{std::nullopt};
//	static_assert(std::is_same<decltype(o7), std::nullopt_t>());
//	static_assert(std::is_same<decltype(o7)::value_type, std::nullopt_t>());

#if VER_ge23 || defined(__cpp_lib_optional)
	// XXX TODO XXX
	// and_then
		// If *this contains a value, invokes f with the contained value as an argument, and returns the result of that invocation; otherwise, returns an empty std::optional.
	// or_else
		// Returns *this if it contains a value. Otherwise, returns the result of f.
	// transform
		// If *this contains a value, invokes f with the contained value as an argument, and returns an std::optional that contains the result of that invocation; otherwise, returns an empty std::optional.
#endif
}

//--------------------------------------------------------------------------------------------------//
//--------------------------------------------------------------------------------------------------//
//--------------------------------------------------------------------------------------------------//

void test__std_variant__retrieveManagedValue()
{	PRenteredFU;
	unsigned const input = 77U;
	using yaVari_t = std::variant<std::string,unsigned const,float,bool>;
	yaVari_t vx{input};
	auto x0 = std::get<decltype(input)>(vx);
	auto x1 = std::get<1>(vx);
	auto x2 =*std::get_if<decltype(input)>(&vx);
	auto x3 =*std::get_if<1>(&vx);
//	auto x4 = std::move(vx);   //Nope!  That just produces another std::variant<> obj.
	assert(input==x0);
	assert(input==x1);
	assert(input==x2);
	assert(input==x3);
//	assert(input==x4);
}


void test__std_variant__checkTenantType()
{	PRenteredFU;

	std::variant<unsigned,long,double,char const*,bool> vObj;

	// Some cpiletime helpers
	static_assert(std::variant_size_v<decltype(vObj)> == 5);
	//
	static_assert(std::is_same_v<  double ,
	                               std::variant_alternative_t<2, decltype(vObj)>  >);
	vObj = 42U;
	SAYevalCHKretBOOL( std::holds_alternative<unsigned>(vObj)     ,true);
	SAYevalCHKretBOOL( vObj.index() == 0                          ,true);

	vObj = "bunnies";
	SAYevalCHKretBOOL( std::holds_alternative<char const*>(vObj)  ,true);
	SAYevalCHKretBOOL( vObj.index() == 3                          ,true);
}


void test__std_variant__other()
{	PRenteredFU;
	// Hey let's all blindly trust the compiler!
	std::variant<float, bool, int64_t, signed char const, char const *> v0;

	assert(v0.index() == 0);

	v0 = false; assert(v0.index() == 1);

	v0 = (uint16_t)42;
	PRmsg("Handed u16, %zu\n", v0.index()); // chose int64_t

	v0 = (uint8_t)42;
	PRmsg("Handed u8, %zu\n", v0.index()); // chose int64_t

//	v0 = (uint64_t)42;       // Err!
//	v0 = (uintptr_t)42;       // Err!

	v0 = (bool const)false;  // chose bool
//	v0 = (signed char)'Z';  // Err
	// Feels free to discard arg's constness, but not type param's constness.

    // Default ctor value-initializes the first (i.e. index=0) alternative,
    std::variant<long, std::string> v1;
    assert(std::holds_alternative<long>(v1)   &&
           v1.index() == 0                   &&
           std::get<long>(v1) == 0L);

	// In-place, with in_place_type,
    std::variant<std::string,float> v2(std::in_place_type<std::string>, 3, 'A');
	// In-place, with in_place_index,
    std::variant<float,std::vector<bool>,std::string,unsigned>
	                                v3(std::in_place_index<2>, 33, 'Z');

	// Ret ptr to stored val, or nullptr if err.  For whatev reason, also *takes* ptr.
	std::string const* pstring;
	pstring = std::get_if<std::string>(&v2);
	assert(  pstring);
	v2.emplace<float>(1.1f);
	pstring = std::get_if<std::string>(&v2);
	assert(! pstring);

	// Normally some kind of value *alw* present; but may become valueless if throws during
	// move assign or type-changing emplace, etc.
	assert(! v1.valueless_by_exception());

	// How to ensure a variant is default-cstructible, even if its cstitients aren't.
	//
//	std::variant<               NotDefaultConstructible,float> v5;  //Err: cannot default-cstruct 1st type!
	std::variant<std::monostate,NotDefaultConstructible,float> v6;  //Dummy type to the rescue.
}


void test__std_variant__visitCall()
{	PRenteredFU;
	// visit: inflict a callable on curr occupant.
	//  Visitors are objects that have to unambiguously provide a function call operator for each possible type.
	auto visA = [](auto&& xarg) -> int {
		return fuPRmsg("A xarg '%lld'  sizeof=%zu\n", (long long)(xarg), sizeof(xarg));
	};
	auto visB = [](auto const& xarg) -> void {
		fuPRmsg("B xarg '%lld'  sizeof=%zu\n", (long long)(xarg), sizeof(xarg));
	};
	OloadedFuCallOp ofco;
	//
	std::variant<unsigned,long,double,char const*,bool> fallow_vObj;
	using vObjType = decltype(fallow_vObj);
	std::vector<decltype(fallow_vObj)> vec = {77U, 33L, 3.14, "bunnies", true};
	for (auto& some_vObj : vec) {
		puts("-------------------");
		size_t const idx = some_vObj.index();
		PRmsg("Now, vObj holding %zuth poss type\n", idx);
//		PRmsg("Now, %s\n", typeid(typename std::variant_alternative_t<idx, decltype(fallow_vObj)>).name());
		std::visit(visA, some_vObj); // returns int
		std::visit(visB, some_vObj); // returns void
		std::visit(ofco, some_vObj);
#if NULL ///////// and this ought to work... but doesn't.
#	if VER_ge20 // Further, 20+ gives "invoke_r" spiffed-up version of visit().
//		int const retint = std::visit(obrt, some_vObj);
		float const retfloat = std::visit<float,OloadedByRetType,decltype(some_vObj)>(obrt, some_vObj);
#	endif
#endif
	}
}


void test__std_variant__derivingFrom()
{	PRenteredFU;
	//TODO
}


struct ZenMonk {
	void                  mkSound ()                 const {}
};
struct Cat {
	double                mkSound ()                 const {
		puts("Meyyowww");  return 3.14159;   }
};
struct Dog {
	static void           mkSound (uint16_t bone = 42)     {
		puts("Arf,arff");  }
};
struct CircularSaw {
	std::pair<char,float> mkSound ()                 const {
		puts("Eeyyowww");  return std::make_pair('Z',35.6F);  }
};

using Polyphonic = std::variant<Cat, Dog, CircularSaw, ZenMonk>;

void test__std_variant__aKindOfPolymorphism()
{	PRenteredFU;
	std::vector<Polyphonic> vec;
	vec.push_back(Dog{});
	vec.push_back(CircularSaw{});
	vec.push_back(Cat{});
	vec.push_back(CircularSaw{});
	vec.push_back(ZenMonk{});
	vec.push_back(CircularSaw{});
	vec.push_back(Cat{});
	//
	for (Polyphonic const& vecElem : vec) {
		std::visit(
					[] (auto const& audible) { audible.mkSound(); } //a generic lambda
					,
					vecElem);
	}
}

//--------------------------------------------------------------------------------------------------//
//--------------------------------------------------------------------------------------------------//
//--------------------------------------------------------------------------------------------------//

void test__std_any__retrieveManagedValue()
{	PRenteredFU;
	unsigned const input = 77U;
	std::any ax{input};
	auto x0 = std::any_cast<decltype(input)>(ax);
//	auto x1 = std::move(ax);   // Nope!  That just produces another std::any obj.
	assert(input==x0);
//	assert(input==x1);
}


void test__std_any__checkTenantTypeAndPresence()
{	PRenteredFU;
	unsigned const input = 77U;
	std::any ax{input};
	assert(ax.has_value());
	assert(ax.type() == typeid(decltype(input)));
	ax.reset();
	assert(!ax.has_value());
}


void test__std_any__other()
{	PRenteredFU;
	std::any        a0; // Empty
	assert(! a0.has_value());
	a0 = 4.2f;
	assert(  a0.has_value());
	a0.reset();
	assert(! a0.has_value());

	std::any a1(4.2f);
	std::any a2(NULL);
	std::any a3('z');
	std::any a4(0x0);
	std::any a5(-42U);

	// Wondering what type got deduced?
	printf("Turns out, contents of a1 typed =  %s\n", a1.type().name());
	printf("Turns out, contents of a2 typed =  %s\n", a2.type().name());
	printf("Turns out, contents of a3 typed =  %s\n", a3.type().name());
	printf("Turns out, contents of a4 typed =  %s\n", a4.type().name());
	printf("Turns out, contents of a5 typed =  %s\n", a5.type().name());

	std::any a10{42};
	assert(a10.type() == typeid(int));
	// Get the literal's ype; if want special treatment, say so with in_place_type
	std::any a11{ std::in_place_type<long>, 42 };
	// ...or, you know, [gasp] cast it maybe?
	std::any a12{(long)42};
	assert(a12.type() == typeid(long));
	std::any a13{42L};
	assert(a13.type() == typeid(long));
	std::any a14{"bunnies"};
	assert(a14.type() == typeid(char const*));
	std::any a15{(std::string) "bunnies"};
	assert(a15.type() == typeid(std::string));

	// Deposited values decay: lose their constness/refness; and arrays become ptrs.
	std::string const sIn{"chair"};
	std::any a16{sIn};
	std::string& sOut = std::any_cast<std::string&>(a16);
	sOut += "man";
	PRmsg("And now now sIn=\"%s\"\n", sIn.c_str());
	// Luckily, value semantics ==> everything copied (expensive BTW), so sIn unchanged.

	// Actually getting the value **back out**: need any_cast.
	auto contents_of_a5 = std::any_cast<unsigned>(a5); // If type wrong, throws.  (What, thought it'd be signed eh?)

	// In-place cstru, with in_place_type,
    std::any a6(std::in_place_type<std::string>, 7, 'A');
	//
	a6.emplace<std::string>(77, 'B'); // same deal

	// If type wrong, won't cpile.
	auto a7 = std::make_any<long>(-42U);
	assert(a7.type() == typeid(long));
}



int main() {
	test__std_optional__retrieveManagedValue();
	test__std_optional__checkTenantPresence();
	test__std_optional__other();
	test__std_variant__retrieveManagedValue();
	test__std_variant__checkTenantType();
	test__std_variant__derivingFrom();
	test__std_variant__visitCall();
	test__std_variant__other();
	test__std_variant__aKindOfPolymorphism();
	test__std_any__retrieveManagedValue();
	test__std_any__checkTenantTypeAndPresence();
	test__std_any__other();

	void relativeSizeofs (void);
	relativeSizeofs();
}


#include <memory>
void relativeSizeofs (void)
{	PRenteredFU;
	std::string empty_s;
	std::string s{"z"};
	std::string *ps = &s;
	std::unique_ptr<std::string> ups = std::make_unique<std::string>("z");
	std::optional<std::string> os{"z"};
	PRmsg("SZ |empty_s|=%zu |s|=%zu |ps|=%zu |ups|=%zu |os|=%zu\n",
	      sizeof empty_s, sizeof s, sizeof ps, sizeof ups, sizeof os);
}
