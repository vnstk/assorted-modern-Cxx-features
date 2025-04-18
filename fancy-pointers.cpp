/*   (c)2024 Vainstein K.   */
#include "common.h"
//#include <utility> // For std::pair
//#include <tuple>
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

struct AlsoNotDefaultConstructible {
	int _i;
	AlsoNotDefaultConstructible (int i) : _i(i) {}
	AlsoNotDefaultConstructible() =delete;
};


#if VER_ge17


void test___std_optional()
{	PRenteredFU;
	std::optional<std::string> o1(std::in_place, {'a', 'b', 'c'});
	std::optional<std::string> o2(std::in_place, 5, 'Z');
	std::string s0{"bunnies"};
	std::optional<std::string> o3(std::move(s0));
	const std::string s1{"bunnies"};
	std::optional<std::string> o4(          s1 );

	o4.emplace(17, 'Q'); // replace with std::string cstructed from these args, fwded.

	// YA way to cstruct an obj && wrap it in an optional,
	std::optional<std::string> o5 = std::make_optional<std::string>(11,'J');
	auto                       o6 = std::make_optional<std::string>(11,'J');

	printf("(Ln%d) ctains a value?  %s\n",__LINE__, (bool)o1?"yea":"nay");
	printf("(Ln%d) ctains a value==>  \"%s\"\n",__LINE__, o1.value_or("sadness").c_str());
	o1.reset();
	printf("(Ln%d) still ctains a value?  %s\n",__LINE__, (bool)o1?"yea":"nay");//Should't.
	printf("(Ln%d) ctains a value==>  \"%s\"\n",__LINE__, o1.value_or("sadness").c_str());

	printf("(Ln%d) o2 ctains \"%s\"\n",__LINE__,  o2->c_str());
	printf("(Ln%d) o2 ctains \"%s\"\n",__LINE__,(*o2).c_str());

	//Uninit optional has special type nullopt_t...
	std::optional<std::string> o7{std::nullopt};
//	static_assert(std::is_same<decltype(o7), std::nullopt_t>());
	std::optional<std::string> o8{std::nullopt};
//	static_assert(std::is_same<decltype(o8), std::nullopt_t>());
	// ...except it doesn't when I tried.

#if VER_ge23 || defined(__cpp_lib_optional)
	puts("Monadic operations in std::optional");
	// XXX TODO XXX
	// and_then
		// If *this contains a value, invokes f with the contained value as an argument, and returns the result of that invocation; otherwise, returns an empty std::optional.
	// or_else
		// Returns *this if it contains a value. Otherwise, returns the result of f.
	// transform
		// If *this contains a value, invokes f with the contained value as an argument, and returns an std::optional that contains the result of that invocation; otherwise, returns an empty std::optional.
#endif
}


void test__std_variant()
{	PRenteredFU;
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

	std::variant<unsigned,long,double,char const*,bool> v4;

	// Some cpiletime helpers
	static_assert(std::variant_size_v<decltype(v4)> == 5);
	//
	static_assert(std::is_same_v<  double ,
	                               std::variant_alternative_t<2, decltype(v4)>  >);

	// visit: inflict a callable on curr occupant.
	auto visA = [](auto&& xarg) -> int {
		return
		printf("/%s/A  xarg \"%lld\" |sizeof=%zu\n", currFU, (long long)(xarg), sizeof(xarg));
	};
	auto visB = [](auto const& xarg){
		printf("/%s/B  xarg \"%lld\" |sizeof=%zu\n", currFU, (long long)(xarg), sizeof(xarg));
	};
	OloadedFuCallOp ofco;
	//
	std::vector<decltype(v4)> vec_of_v4 = {77U, 33L, 3.14, "bunnies", true};
	for (auto& some_v4 : vec_of_v4) {
		puts("-------------------");
		std::visit(visA, some_v4);
		std::visit(visB, some_v4);
		std::visit(ofco, some_v4);
#if NULL ///////// and this ought to work... but doesn't.
#	if VER_ge20 // Further, 20+ gives "invoke_r" spiffed-up version of visit().
//		int const retint = std::visit(obrt, some_v4);
		float const retfloat = std::visit<float,OloadedByRetType,decltype(some_v4)>(obrt, some_v4);
#	endif
#endif
	}

	// How to ensure a variant is default-cstructible, even if its cstitients aren't.
	//
//	std::variant<               NotDefaultConstructible,float> v5;  //Err: cannot default-cstruct 1st type!
	std::variant<std::monostate,NotDefaultConstructible,float> v6;  //Dummy type to the rescue.
}


void test__std_any()
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

	auto contents_of_a5 = std::any_cast<unsigned>(a5); // If type wrong, throws.  (What, thought it'd be signed eh?)

	// In-place cstru, with in_place_type,
    std::any a6(std::in_place_type<std::string>, 7, 'A');
	//
	a6.emplace<std::string>(77, 'B'); // same deal

	// If type wrong, won't cpile.
	auto a7 = std::make_any<long>(-42U);
	assert(a7.type() == typeid(long));
}


#endif // VER_ge17

int main() {
#if VER_ge17
	test___std_optional();
	test__std_variant();
	test__std_any();
#endif
}
