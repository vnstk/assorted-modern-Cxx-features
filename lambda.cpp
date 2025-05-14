/*   (c)2024 Vainstein K.   */
#include "common.h"
#include <utility> //For   as_const()
#if VER_ge20
#	include <source_location>
#endif


/*
lt 20,   Lambda expressions without an explicit template parameter list (possibly non-generic)

ge 20,   Lambda expressions with an explicit template parameter list (always generic)
*/

struct Foo {
	unsigned _x{42};
	float _y{3.14159};
};

struct Bar {
	int _x{42};
	double _y{3.14159};
};


#if VER_ge20
/* note, no "template" keywd!*/
auto genla1 = [] <typename T> (T targ) -> bool { return (double)targ._x > targ._y; };
#endif



struct S2
{
	unsigned _u{42};
	//
	float f (int i); // Only invoked from test__capture().
};

float S2::f (int i)
{
	uint8_t eig = 0xFE;
	fuPRmsg("Ini, i=%d _u=%u\n", i, this->_u);

	// of course, can capture this explicitly.
	[this] { this->_u += 3U; }();

	// by-copy capture default; with either capture default, have access to "this".
		// But note, such "implicit this capture" is deprec in 20+.
#if ! VER_ge20
	[=] { this->_u += 3U; }();
#endif
//	[=] { i += 10; };          // Err: i was captured by copy, hence is rdonly.
//	[=] { ++eig;   };          // Err: eig was captured by copy, hence is rdonly.
//	But, not all is lost!!  Behold, mutable removes const qual from params captured by copy:
	PRmsg("Before, i is yet %d\n",i);
	[=] () mutable { i += 10;   fuPRmsg("Within, i changed to %d\n",i); }();
	PRmsg("Without, i is still %d\n",i);
	/* Not that the lambda marked mutable is, itself, mutable.  But, what with move
	semantics, now const ivars are largely a thing of the past; so the once-so-useful
	mutable now doesn't apply much anymore, so we have this "mutable" keyword freed up!  */

	// by-copy capture, except i is captured by reference
	[=, &i] { i ^= 0x3030303;   fuPRlit("Hi from LA!  mbda"); }();
//	[=, &i] { ++eig; }         // Err, still.

	// by-ref capture default; with either capture default, have access to "this".
	[&] { this->_u += 3U; }();

	PRmsg("Did that lambda run??  i=%d _u=%u eig=%hu\n",i,_u,eig);


#if VER_ge17       // capture the enclosing S2 by copy.
    [=, *this] { const unsigned flarp = 5U + this->_u; }();//fine
//	[=, *this] { this->_u += 5U; }();                //Err, because "this" by copy, hence rdonly!
	// Pity about copying the possibly-huge object just so it's rdonly inside lambda, but oh well.
#endif

	PRmsg("Did that lambda run??  i=%d _u=%u eig=%hu\n",i,_u,eig);
#if VER_ge17       // capture enclosing S2 by copy; but "this" is *not* rdonly.
    [=, this] { const unsigned flarp = 5U + this->_u; }();//fine
	[=, this] { this->_u += 5U; fuPRlit("howdy"); }(); 
	// So then, why'd we pay to do the copy??
#endif
	PRmsg("Did that lambda run??  i=%d _u=%u eig=%hu\n",i,_u,eig);

	return (float)i + 3.14F;
}


#if VER_ge20
template<typename T1, typename T2>
std::common_reference<T1,T2>::type fAddCommon_b (T1 x, T2 y) {
	return x + y;
}
// generic lambdas: "auto"-typed params automagically "genericize".
auto fAddCommon_c = [] (auto x, auto y) { return x + y; };
#endif
//
// fAddCommon_b and fAddCommon_c should be utterly equiv. Except latter is less readable.

void test__auto_retType()
{	PRenteredFU;
	//
#if VER_ge20
	auto retCommon_b = fAddCommon_b(42L, 3.14f);
	PRtyp(retCommon_b);
	auto retCommon_c = fAddCommon_c(42L, 3.14f);
	static_assert(std::is_same_v<decltype(retCommon_b), decltype(retCommon_c)>);
	PRwhat(retCommon_b);
	PRwhat(retCommon_c);
	assert(retCommon_b == retCommon_c);
#endif
}


void test__generic_lambda()
{	PRenteredFU;
#if VER_ge14           // generic lambda, style 1: "auto" as type of a param.
	auto glambda = [](auto a, auto&& b) { return a < b; };
	bool b = glambda(3, 3.14); // OK
#endif
}


void test__static_call_operator()
{	PRenteredFU;
	// static operator() ... curious, what would be useful for?
#if VER_ge23 || defined(__cpp_static_call_operator)
	auto stalamb = [](Foo const& crFoo) static { return crFoo._x + 10; };
	unsigned ret_stalamb = stalamb(xa);
	PRmsg("ret_stalamb=%u\n", ret_stalamb);
#endif
}


void test__capture()
{	PRenteredFU;

	S2 z;
	const float zret = z.f(4242);

	Foo xa;
	double doub{1.0001};

#if VER_ge14  // capture *initializers*, use for just aliases and/or to modif.
	[& rdoub = doub, zdoub = doub + 3.003]() {
		rdoub += 2.002; fuPRmsg("rdoub=%f zdoub=%f\n",rdoub,zdoub);
	}();

	// Lets capture rvals
	[z = std::move(xa)]() { fuPRmsg("xa._z=%u\n",z._x); }();

#	if VER_ge17
	// And, with as_const, can spare the terrible terrible inconvenience of creating a const-ref:
	[z = std::as_const(xa)]() { fuPRmsg("xa._z=%u\n",z._x); }();
#	else
	decltype(xa) const& constref_to_xa = xa;
	[z = constref_to_xa]() { fuPRmsg("xa._z=%u\n",z._x); }();
#	endif

#endif
}


void test__lambda_source_loc()
{	PRenteredFU;
#if VER_ge20
	[]() {
		std::source_location sloc = std::source_location::current();
		printf("source_location %s:%u,%s\n", sloc.file_name(),sloc.line(),sloc.function_name());
		fuPRmsg("source_location %s:%u,%s\n", sloc.file_name(),sloc.line(),sloc.function_name());
	}();
	auto slamb = [](float& reff) -> Foo* {
		printf("In slamb, %s, %s, was reff=%.3f\n", __func__,__FUNCTION__, reff);
		// __func__ prints  "operator()"; __FUNCTION__ same.
		reff *= 1.1f;
		std::source_location sloc = std::source_location::current();
		printf("slamb source_location %s:%u,%s\n", sloc.file_name(),sloc.line(),sloc.function_name());
		// function_name() prints  "main()::<lambda(float&)>"  , an improvement indeed!
		return nullptr;
	};
	float flambee{3.14};
	slamb(flambee);
	slamb(flambee);
	slamb(flambee);
#endif
}



int main () {
	test__auto_retType();
	test__lambda_source_loc();
	test__capture();
	test__generic_lambda();
	test__static_call_operator();
}

// Moved constexpr lambdas examples into see constexpr.cpp
