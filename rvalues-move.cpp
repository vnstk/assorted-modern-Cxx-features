/*   (c)2024 Vainstein K.   */
#include "common.h"
#include <utility> // For std::forward std::move_if_noexcept etc
#include <vector>
#include <string>


struct TypA { /// trivial everything, no special membs defined, no special membs del
	float _f{0.11};
	int64_t _i{11};
};
struct TypB { /// defined ctor
	float _f{0.22};
	int64_t _i{22};
	TypB (float ff) : _f(ff) {}
};
struct TypC { /// defined ctor & marked it explicit
	float _f{0.33};
	int64_t _i{33};
	explicit TypC (float ff) : _f(ff) {}
};
struct TypD { /// defined a no-arg ctor; defined a might-throw move ctor
	float _f{0.22};
	int64_t _i{22};
	TypD (float ff) : _f(ff) {}
	TypD (TypD&& rhs) : _f(rhs._f), _i(rhs._i) {}
};
struct TypE {
	float _f{0.22};
	int64_t _i{22};
	TypE (float ff) : _f(ff) {}
	TypE (TypE&& rhs) : _f(rhs._f), _i(rhs._i) {}              // move ctor: might-throw
	TypE (const TypE& rhs) noexcept : _f(rhs._f), _i(rhs._i) {}// copy ctor: might-throw
};
struct TypF {
	float _f{0.22};
	int64_t _i{22};
	TypF (float ff) : _f(ff) {}
	TypF (TypF&& rhs) noexcept : _f(rhs._f), _i(rhs._i) {} // move ctor: can't-throw
	TypF (const TypF& rhs) : _f(rhs._f), _i(rhs._i) {}     // copy ctor: might-throw
};
struct TypG {
	float _f{0.22};
	int64_t _i{22};
	TypG (float ff) : _f(ff) {}
	TypG (TypG&& rhs) =delete;                    // move ctor: deleted.
	TypG (const TypG& rhs) =default;              // copy ctor: defaulted.
};
struct TypH {
	float _f{0.22};
	int64_t _i{22};
	TypH (float ff) : _f(ff) {}
	TypH (TypH&& rhs) : _f(rhs._f), _i(rhs._i) {} // move ctor: might-throw.
	TypH (const TypH& rhs) =default;              // copy ctor: defaulted.
};

struct TypP {
	unsigned _u{42U};
	TypP (unsigned u) : _u(u) {}
	TypP (TypP&& rhs) : _u(rhs._u) {
		printf("\tin TypP move ctor, _u=%u\n", _u);
	}
	TypP& operator= (TypP&& rhs) {
		printf("\tin TypP move assignm, _u=%u rhs._u=%u\n", _u, rhs._u);
		if (&rhs != this) {
			_u = rhs._u;
		}
		return *this;
	}
};
static void dumpvec (const std::vector<TypP>& vec, const char *const tag) {
	printf("\tDumping '%s':  {", tag);
	for (int i = 0; i < vec.size(); ++i) {
		printf(" [%d]=%u", i, vec[i]._u);
	}
	printf(" }  //sz=%zu\n", vec.size());
}


template<typename T>
void f0 (T& targ) {
}
template<typename T>
void f1 (const T& targ) {
}
template<typename T>
void f2 (T&& targ) {
}
template<typename T>
void f3 (const T&& targ) {
}

template<typename T>
void g (T& targ) {
	PRwhat(targ);
	printf("\tIn 'T&' oload\n\n");
}
template<typename T>
void g (const T& targ) {
	PRwhat(targ);
	printf("\tIn 'const T&' oload\n\n");
}
template<typename T,
#if VER_eq11
         typename std::enable_if<std::is_nothrow_copy_constructible<T>::value>::type
#elif VER_eq14
         std::enable_if_t<std::is_nothrow_copy_constructible<T>::value, bool>
//       std::enable_if_t<std::is_nothrow_copy_constructible<T>::value, bool> = true
#elif VER_ge17
         std::enable_if_t<std::is_nothrow_copy_constructible_v<T>>
//       std::enable_if_t<std::is_nothrow_copy_constructible_v<T>> = true
#endif
>
void g (T&& targ) {
	PRwhat(targ);
	printf("\tIn 'T&&' oload\n\n");
#if VER_ge14
	T someLocal{99};
	const T preExchangeVal = std::exchange(someLocal, targ);
#endif
}
template<typename T>
void g (const T&& targ) {
	PRwhat(targ);
	printf("\tIn 'const T&&' oload\n\n");
}

template<typename T>
void forw_to_g (T&& fwdgref) {
	PRwhat(fwdgref);
	printf("In forw_to_g\n");
	g(std::forward<T>(fwdgref));
}


struct TypS { // a simple std::string wrapper
	const std::string _s;
	TypS (const std::string&  s) : _s(s) {   fuPRval(_s.c_str(),"%s");   }
	TypS (      std::string&& s) : _s(s) {   fuPRval(_s.c_str(),"%s");   }
};


int main ()
{
	SAYeval(   TypS{"foo"}   ); //str literal ==> std::string xvalue ==> TypS's move ctor
	std::string foo_strObj{"foo"};
	SAYeval(   TypS{foo_strObj}   );           // std::string lvalue ==> TypS's copy ctor
	puts("-----------------------------------------");


	TypA xa;
	TypB xb{55};
	const TypB cxb{55};
	TypC xc{55};

	TypB& rxb = xb;
	const TypB& crcxb = cxb;
#if 0
	const TypB& crxa = xa;       // Illegal, turns out.
#endif

	forw_to_g(xa);
	forw_to_g(xb);
	forw_to_g(cxb);
	forw_to_g(xb);
	forw_to_g(rxb);
	forw_to_g(crcxb);
	forw_to_g(TypB{66});
	forw_to_g(TypC{66});

	SAYeval(g(TypB{77}));

	SAYeval(g(std::move(cxb)));

	SAYeval(g(std::move(xa)));

	TypA ya;
	const TypA cya;
	TypB yb{55};
	const TypB cyb{55};
	TypD yd{55};
	const TypD cyd{55};
	TypE ye{55};
	const TypE cye{55};
	TypF yf{55};
	const TypF cyf{55};
	TypG yg{55};
	const TypG cyg{55};
	TypH yh{55};
	const TypH cyh{55};

	TypA& rya = ya;
	TypD& ryd = yd;
	TypE& rye = ye;
	const TypA& crcya = cya;
	const TypD& crcyd = cyd;

//#if ! VER_ge14 //because std::exchange would barf
	SAYeval(g(std::move_if_noexcept(yd)));
//#endif
	SAYeval(g(std::move_if_noexcept(ye)));
//#if ! VER_ge14 //because std::exchange would barf
	SAYeval(g(std::move_if_noexcept(yf)));
//#endif
	SAYeval(g(std::move_if_noexcept(yg)));
	SAYeval(g(std::move_if_noexcept(yh)));

	SAYeval(g(std::move_if_noexcept(cyd)));
	SAYeval(g(std::move_if_noexcept(cye)));
	SAYeval(g(std::move_if_noexcept(cyf)));
	SAYeval(g(std::move_if_noexcept(cyg)));
	SAYeval(g(std::move_if_noexcept(cyh)));

	std::vector<TypP> veca;
	veca.reserve(6);
	veca.emplace_back(11U);
	veca.emplace_back(22U);
	veca.emplace_back(33U);
	veca.emplace_back(44U);
	veca.emplace_back(55U);
	decltype(veca)::iterator itveca = veca.begin();
	std::advance(itveca, 3);
	printf("|veca|=%zu ; itveca->_u=%u\n", veca.size(), itveca->_u);
	dumpvec(veca, "veca");
	//
	std::vector<TypP> vecb;
	vecb.reserve(6);
	vecb.emplace_back(66U);
	std::move(itveca, veca.end(), std::back_inserter(vecb));
	dumpvec(vecb, "vecb");
	//
	std::vector<TypP> vecc;
	veca.reserve(8);
	vecc.emplace_back(71U);
	vecc.emplace_back(72U);
	vecc.emplace_back(73U);
	vecc.emplace_back(74U);
	vecc.emplace_back(75U);
	itveca = veca.begin();
	std::advance(itveca, 3);
	std::move_backward(itveca, veca.end(), vecc.end());
	dumpvec(vecc, "vecc");
	//
	decltype(vecc)::iterator itvecc = vecc.begin();
	++itvecc;
	std::move_iterator<decltype(itvecc)> moveitvecc = std::make_move_iterator(itvecc);
	const TypP z_from_vecc1 = *moveitvecc;
	++moveitvecc;
	const TypP z_from_vecc2 = *moveitvecc;
	++moveitvecc;
	const TypP z_from_vecc3 = *moveitvecc;
	printf("Moved from vecc, with move_iterator: z...1=%u z...2=%u z...3=%u\n",
	       z_from_vecc1._u, z_from_vecc2._u, z_from_vecc3._u);
	

	TypA x0;
	const TypA x1;
#if VER_ge17 // as_const creates a const-ref lvalue; but this lvalue is typed "auto&&" ????
	auto&& rx0 = std::as_const(x0);
	auto&& rx1 = std::as_const(x1);
	PRwhat(rx0);
	PRwhat(rx1);
//	rx0._f *= 1.3579;                //Err: rx0 is a read-only obj.
//	rx1._f *= 1.3579;                //Err: rx1 is a read-only obj.
	TypA& plainrefx0 = x0;
	const TypA& plainconstrefx1 = x1;
	auto&& rx00 = std::as_const(plainrefx0);
	auto&& rx11 = std::as_const(plainconstrefx1);
	PRwhat(rx00);
	PRwhat(rx11);
/*  as_const is shortcut for
                              static_cast<const MyType&>(my_value)
	; but why not just declare a const ref to what you want?
*/
#endif
	const TypA& naivecref = x0;
}



#if 0
		TODO,  https://en.cppreference.com/w/cpp/utility/forward_like   (23)
#endif
