/*   (c)2024 Vainstein K.   */
#include "common.h"
#include <utility>
#include <functional>
#include <tuple>

struct Foo {
	unsigned _u{42U};
	bool     _b{true};
	char     _ch{'V'};
	Foo () =default;
	constexpr Foo (unsigned u, bool b) : _u(u), _b(b) {}
	//
	int f0p (float fp) { return -(int)(fp); }
	int f0c (float fp) const { return -(int)(fp); }
	static int f0s (float fp) { return -(int)(fp); }
	//
	int const& f1p (float);
	int const& f1c (float) const;
	static const int& f1s (float);
	//
	int *f2p (float);
	int *f2c (float) const;
	static int *f2s (float);
};

Foo xa{33,true};
const Foo xb{33,true};
constexpr Foo xc{33,true};

constexpr
Foo			fg0 (float fp,char) {
                         return fp > 1.0F ? Foo{22,true} : Foo{33,false}; }
constexpr
Foo *		fg1 (float,char ch) { return ch > 'q' ? &xa : nullptr; }
constexpr
const Foo *	fg2 (float fp,char ch) {
                         return fp > 1.0F && ch > 'q' ? &xb : nullptr; }

Foo&		fg3 (float);
const Foo&	fg4 (float);
Foo&&		fg5 (float);
double		fg6 (const Foo&&);
constexpr
long		fg7 (unsigned u,float fp, const Foo& crFoo) {
                         return u > 7U && fp > 7.7F && crFoo._ch > '7' ? 77L : -777L; }
double		fg8 (Foo&);
double		fg9 (Foo&&);

constexpr double fgco0 (float fp) { return fp * 1.0001; }


struct FuncObj {
	int operator() (float fp) const noexcept {
		return -(int)(fp);
	}
};

struct Bar {
	int operator() (float) const;
	void operator() (float);
	float operator() (unsigned, std::pair<bool,double>&);
};	

struct Qux {
	int _ii{5};
	float qfA (unsigned u) { return 1.1F * (u + _ii); }
	float qfB (unsigned u) { return 2.2F * (u + _ii); }
	static float qfC (unsigned u) { return 3.3F * (u); }
	static float qfD (unsigned u) { return 4.4F * (u); }
	Qux (double day) : _ii(day > 0.0 ? 10 : -10) {}
};


void test__move_only_function () {
	PRenteredFU;
	// move-only... not needed??
	auto lamb0 = [](char ch)noexcept ->float{return 1.01F*ch;} ;
	std::function<float(char)> fuA{ std::move(lamb0) };
	std::function<float(char)> fuB = std::move(lamb0) ;
	auto lamb1 = [](char ch){return 1.01F*ch;} ;
#if VER_ge23 || defined(__cpp_lib_move_only_function)
	std::move_only_function<float(char)> fuC = std::move(lamb1) ;
	PRwhat(fuC);
	PRmsg("invoked move_only_function, %f\n",__LINE__,fuC('W'));
#endif
}


void test__std_apply ()
{	PRenteredFU;
	//
	const Foo* retA_fg2 = fg2(4.44F, 'q');
	std::tuple<float,char> tupA = std::make_tuple(4.44F, 'q');
#if VER_ge17 // providing args as a tuple
	const Foo* retB_fg2 = std::apply(fg2, tupA);
#endif
}



void test__invoke__invoke_r__result_of (void)
{	PRenteredFU;

	const Foo* retC_fg2 = fg2(4.44F, 'q');
#if VER_ge17
	// special call to invoke, instead of just invoking.  No clue why.
	const Foo* retD_fg2 = std::invoke(fg2, 4.44F, 'q');
	auto       retE_fg2 = std::invoke(fg2, 4.44F, 'q');
	// NB, with invoke() needn't specify ret type as a te arg.
#endif
#if VER_ge23 || defined(__cpp_lib_invoke_r)
	// Big attraction of invoke_r is that ret type participates in oload
	// resolution; so can dictate type by giving ret type!
	constexpr auto lambJ = [](int32_t x, int16_t y) { return x + y; };
	//
	auto retJ0 = std::invoke_r<float>(lambJ, 7,8);
	static_assert(std::is_same<decltype(retJ0), float>());
	//
	auto retJ1 = std::invoke_r<bool>(lambJ, 8,9);
	static_assert(std::is_same<decltype(retJ1), bool>());
	//
	printf("(Ln%d)  retJ0=%f retJ1=%f\n",__LINE__, retJ0,retJ1);

	// Downside is, must alw specif ret type.
	auto       retF_fg2 = std::invoke_r<const Foo*>(fg2, 4.44F, 'q');
//	const Foo* retG_fg2 = std::invoke_r(fg2, 4.44F, 'q'); //No ret typ specifd!
	// ...OK to specify in both places,
	const Foo* retH_fg2 = std::invoke_r<const Foo*>(fg2, 4.44F, 'q');
//	auto       retQ_fg2 = std::invoke_r(fg2, 4.44F, 'q'); //No ret typ specifd!
#endif

	// Deduce ret type of invocation w/o actually invoking; note,
	// methods thus referred to needn't've been implemented, even!
#if VER_ge17
//	std::invoke_result<Bar,float>::type rettype0 = 42;  //Err! deduces void
	std::invoke_result<const Bar,float>::type rettype1 = 42; 
	static_assert(std::is_same<
	                             std::invoke_result<const Bar,float>::type
	                           , int>());
	//
	std::invoke_result<Bar,unsigned,std::pair<bool,double>&>::type rettype2 = 4.2;
#endif
#if ! VER_ge20 // 11 through 14, deprec in 17, removed in 20, another syntax:
//	std::result_of<Bar(float)>::type rettype3 = 42;  //Err! deduces void
	std::result_of<const Bar(float)>::type rettype4 = 42; 
	static_assert(std::is_same<
	                             std::result_of<const Bar(float)>::type
	                           , int>());
	//
	std::result_of<Bar(unsigned,std::pair<bool,double>&)>::type rettype5 = 4.2;
#endif
}


void test__the_many_ways_to_bind()
{	PRenteredFU;

	// free fu
	std::function<double(float)> fu3 = fgco0;

	// lambda (wrapping in std::function might cause unnecessary overhead)
	std::function<char(float)> fu4{ [](char ch) noexcept {return 1.01F*ch;} };

	// utterly gratuitous wrapping
	std::function<char(float)> fu4_even_more{ fu4 };

	// func obj
	std::function<int(float)> fu5 = FuncObj{};
	(int) fu5(7.77F);

	// static memb meth
	std::function<int(float)> fu6 = Foo::f0s;
	(int) fu6(4.44F);
	(int) fu6(5.55F);

	// instance memb meth
	std::function<int(Foo&,float)> fu7 = &Foo::f0p;
	fu7(xa, 3.333F); // Just another...
	xa.f0p(3.333F);  // ...way to say it!
	std::function<int(const Foo&,float)> fu8 = &Foo::f0c;
	fu8(xb, 3.333F);


    using std::placeholders::_1;
    using std::placeholders::_2;
	std::function<int(float)> fu9_b0 = std::bind(&Foo::f0c,  xb, _1);
	fu9_b0(3.33F); //// Exact equiv of "fu8(xb, 3.333F);"  , itself equiv of
	               ////                "xb.f0c(3.33F);"
	//Curious: ptr to instance works same,
	std::function<int(float)> fu9_b1 = std::bind(&Foo::f0c, &xb, _1);
	PRwhat(fu9_b1);
	PRwhat(                            std::bind(&Foo::f0c, &xb, _1)   );
	fu9_b0(3.33F);

	//NB: te params to the result of std::bind must match the eventual invocation,
	// and *not* the original func we'd bound!  Thus, use "float,const Foo&" in te
	// args of fg7_bA --- even though fg7() takes "unsigned,float,const Foo&".
	std::function<long(float,const Foo&)> fg7_bA = std::bind(&fg7, 99U, _1, _2);
	fg7_bA(9.9F, xb);
	// Don't forget: that's same as
	fg7_bA.operator()(9.9F, xb);

	// More concise with auto.
	auto fg7_bB = std::bind(fg7, 99U, _1, _2);
	PRwhat(       std::bind(fg7, 99U, _1, _2)     );
	fg7_bB(9.9F, xb);

	// But wait!  With 20, can even dispense with placeholders (and readability).
#if VER_ge20 || defined(__cpp_lib_bind_front)
	std::function<long(float,const Foo&)> fg7_bD = std::bind_front(fg7, 99U);
	PRwhat(fg7_bD);
	PRwhat(                                        std::bind_front(fg7, 99U)  );
	long ret7D = fg7_bD(9.9F, xb);
	PRmsg("invoked fu obj created by bind_front, %ld\n", ret7D);

	// Or auto,
	auto fg7_bE = std::bind_front(fg7, 99U);
	auto ret7E = fg7_bE(9.9F, xb);
	static_assert(std::is_same<decltype(ret7E), long>());

	// constexpr?  Sure, if funcobj and args and invocat_retval are all constexpr.
	constexpr auto fg7_bF = std::bind_front(fg7, 99U);
	constexpr auto ret7F = fg7_bF(9.9F, xc);
	static_assert(std::is_same<decltype(ret7F), const long>());
	//                                   Yes,  "const long"  and not plain  "long".
#endif

	// But wait!!!!  With 23, can bind Rmost args instead of Lmost ones.
#if VER_ge23 || defined(__cpp_lib_bind_back)
	const Foo& crxa = xa;
	std::function<long(unsigned,float)> fg7_bG = std::bind_back(fg7, crxa);
	long ret7G = fg7_bG(99U, 9.9F);
	PRmsg("invoked fu obj created by bind_back, %ld\n", ret7G);
#endif

	// not_fn: a template to use if typing "!" is too hard ...?
	std::function<int(Foo&,float)> yes__fu_obj = &Foo::f0p;
	PRmsg("With !yes__fu_obj, %s\n", !yes__fu_obj(xa,4.4) ? "qux" : "fnord");
#if VER_ge17
	std::function<int(Foo&,float)> not__fu_obj = std::not_fn(&Foo::f0p);
	PRmsg("With not__fu_obj, %s\n", not__fu_obj(xa,4.4) ? "qux" : "fnord");
	//
	PRwhat(                                                  &Foo::f0p );
	PRwhat(                                      std::not_fn(&Foo::f0p));
#endif

	// Remember pointer-to-member?  (BTW, they work for ivars too, not just membfuncs.)
	float (Qux::* pqf)(unsigned) = nullptr;
	//
	Qux qone{6.99};
	Qux *pqtwo = new Qux{7.01};
	PRwhat(pqtwo);
	//
	pqf = &Qux::qfA;
	PRwhat(pqf);
	float qfret0 = (  qone.*pqf)(33U);
	float qfret1 = (pqtwo->*pqf)(33U);
	pqf = &Qux::qfB;
	float qfret2 = (  qone.*pqf)(33U);
	float qfret3 = (pqtwo->*pqf)(33U);
	// Now we can do that same thing, but **using templates**.
	auto special_pqf = std::mem_fn(&Qux::qfA);
	PRwhat(            std::mem_fn(&Qux::qfA)   );
	float qfret4 = special_pqf(qone, 33U); // Is this better than "(qone.*pqf)(33U)"
	//
	PRmsg("qfret:\n\t'0=%.3f\n\t'1=%.3f\n\t'2=%.3f\n\t'3=%.3f\n\t'4=%.3f\n",
	               qfret0, qfret1, qfret2, qfret3, qfret4);

	// Y'all ain't seen nothin' yet!  Here's a func obj which does ...*nothing* !!
#if VER_ge20
	int fortyTwo{42};
	std::identity doesNothing;
	int also_fortyTwo = doesNothing(fortyTwo);
	PRmsg("fortyTwo=%d also_fortyTwo=%d\n",fortyTwo,also_fortyTwo);
#endif
}



int main () {
	test__move_only_function();
	test__std_apply();
	test__invoke__invoke_r__result_of();
	test__the_many_ways_to_bind();
}
