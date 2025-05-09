/*   (c)2025 Vainstein K.   */
#include "common.h"
#include <functional> //For std::ref et al.

struct Foo {
	char _ch{'Z'};
};



template<typename T>
void f__takesLvalConstRef (T const& r)
{	fuPRmsg("r._ch = %c\n", r._ch);
	PRwhat(r);
	std::reference_wrapper<T const> rw = std::cref(r);
	rw = std::cref(rw); // idempotent
	rw = std::cref(rw); // idempotent, I said
	// Of the 3 expected-illegal lines below, only 2nd and 3rd actually evinced error:
	std::reference_wrapper<T const> aWrong = std::ref(r); // ref() won't wrap T&-const
//	std::reference_wrapper<T>       bWrong = std::cref(r); // cref() won't produce wrapper-of-T&-nonconst
//	std::reference_wrapper<T>       cWrong = std::cref(rw); // cref() won't produce wrapper-of-T&-nonconst
}

template<typename T>
void f__takesLvalNonconstRef (T& r)
{	fuPRmsg("r._ch = %c\n", r._ch);
	PRwhat(r);
	std::reference_wrapper<T> rw = std::ref(r);
	rw = std::ref(rw); // idempotent
	rw = std::ref(rw); // idempotent, I said
	// Of the 3 expected-illegal lines below, only 1st actually evinced error:
//	std::reference_wrapper<T> aWrong = std::cref(r); // cref() won't wrap T&-nonconst
	std::reference_wrapper<T const> bWrong = std::ref(r); // ref() won't produce wrapper-of-T&-const
	std::reference_wrapper<T const> cWrong = std::ref(rw); // ref() won't produce wrapper-of-T&-const
}

template<typename T>
void f__takesRvalConstRef (T const&& r)
{	fuPRmsg("r._ch = %c\n", r._ch);
	PRwhat(r);
	static_assert(std::conjunction<
		std::is_rvalue_reference<decltype(r)>,
		std::is_const<typename std::remove_reference<decltype(r)>::type>>::value);
	std::reference_wrapper<T const> rw = std::cref(r); /* That should have been impossible, since
			template< class T >
			void cref( const T&& ) = delete;            */
	//
	rw = std::cref(rw); // idempotent
	rw = std::cref(rw); // idempotent, I said
	// Of the 3 expected-illegal lines below, only 2nd and 3rd actually evinced error:
	std::reference_wrapper<T const> aWrong = std::ref(r); // ref() won't wrap T&-const
//	std::reference_wrapper<T>       bWrong = std::cref(r); // cref() won't produce wrapper-of-T&-nonconst
//	std::reference_wrapper<T>       cWrong = std::cref(rw); // cref() won't produce wrapper-of-T&-nonconst
}

// The reason f__takesRvalNonconstRef() works is that r **itself**, the arg, is an lval ref. */
template<typename T>
void f__takesRvalNonconstRef (T&& r)
{	fuPRmsg("r._ch = %c\n", r._ch);
	PRwhat(r);
	std::reference_wrapper<T> rw( r );
	rw = std::ref(rw); // idempotent
	rw = std::ref(rw); // idempotent, I said
	// Of the 3 expected-illegal lines below, none actually evinced error:
	std::reference_wrapper<T> aWrong = std::ref(r); // ref() won't wrap T&&-nonconst
	std::reference_wrapper<T const> bWrong = std::cref(r); // cref() won't wrap T&&-nonconst
	std::reference_wrapper<T const> cWrong = std::ref(rw); // ref() won't produce wrapper-of-T&-const
}
	


void test__std_ref__and__std_cref()
{	PRenteredFU;
	Foo a;
	Foo const b;
	PRwhat(a);
	PRwhat(b);
	Foo& c = a;
	Foo const& d = b;
	PRwhat(c);
	PRwhat(d);
	Foo e, f;
	SAYeval( f__takesLvalConstRef(d) );
	SAYeval( f__takesLvalConstRef(c) );
	SAYeval( f__takesLvalNonconstRef(d) );
	SAYeval( f__takesLvalNonconstRef(c) );
	SAYeval( f__takesRvalNonconstRef(std::move(e)) );
	SAYeval( f__takesRvalConstRef(std::move(f)) );
}


void test__unwrapping_the_wrapper()
{	PRenteredFU;
	Foo x, y, z;
	Foo const cx;
	Foo& rx = x;
	Foo const& rcx = cx;
	std::reference_wrapper<Foo>        wx = std::ref(rx);
	std::reference_wrapper<Foo const> wcx = std::cref(rcx);
	
//	std::reference_wrapper<Foo>        wmx = std::ref(std::move(y));
//	std::reference_wrapper<Foo const> wcmx = std::cref(std::move(z));


#ifdef VER_ge20
	// unwrap_reference said to transform refwrap<T> into T&.

	Foo x2;
	Foo const cx2;
	std::unwrap_reference<decltype(wx)>::type urx = x2;
	std::unwrap_reference<decltype(wcx)>::type urcx = cx2;

	static_assert(std::conjunction< // Let's make sure 
		std::is_lvalue_reference<decltype(urx)>,
		std::negation<
			std::is_const<typename std::remove_reference<decltype(urx)>::type>
		>>::value);
	static_assert(std::conjunction< // Let's make sure 
		std::is_lvalue_reference<decltype(urcx)>,
		std::is_const<typename std::remove_reference<decltype(urcx)>::type>
		>::value);
#if 0   //TODO more of
20
std::unwrap_reference
	reference_wrapper<T>  ==>  T&
std::unwrap_ref_decay
https://en.cppreference.com/w/cpp/utility/functional/unwrap_reference
#endif
#endif


}


void test__wrap_something_else()
{	PRenteredFU;
//can change constntess of the wrappee?
}

int main (){
	test__std_ref__and__std_cref();
	test__unwrapping_the_wrapper();
return 0;
#if 0
	float nonconstf;
	PRwhat(nonconstf);
	float const *pf;
	std::remove_pointer<decltype(pf)>::type f = 3.2F;
	PRwhat(pf);
	PRwhat(f);
	float const cf = 3.14F;
	PRwhat(cf);
	float const& crf = cf;
	PRwhat(crf);
	static_assert( std::is_const<decltype(f)>::value );
	float& rf = nonconstf;
	PRwhat(rf);

	Foo x{'Z'};
	PRwhat(x);
	Foo const cx{'Z'};
	PRwhat(cx);
	Foo& rx = x;
	PRwhat(rx);
	Foo const& crx = cx;
	PRwhat(crx);

#endif

}





/*
terminology refresh:
decay means.....  T[]  or  T[]&  ==>   T*    ;  Tfunc  or  Tfunc&  ==>  Tfunc*  .


a)	std::add_rvalue_reference<U>::type //11 
b)	std::add_rvalue_reference<U>          //14+ 

o	ref(t)    rets     std::reference_wrapper<T>(t) ; type T deduced from arg t.
o	cref(t)    rets   std::reference_wrapper<const T>(t) ; type T deduced from arg t.

11
std::ref  , T&  ==> reference_wrapper<T>
std::ref  , T&& ==> reference_wrapper<T>
std::cref , T const&  ==> reference_wrapper<const T>
std::cref , T const&& ==> reference_wrapper<const T>

.get()   or operator T&    ,reference_wrapper<T> ==> T&

std::decay<T>,  get  T*   from   T[]    or T[]&
std::decay<Tfunc>,  get  Tfunc*   from   Tfunc    or Tfunc&



11
std::decay
https://en.cppreference.com/w/cpp/standard_library/decay-copy
	NB:		All usages of decay-copy in the standard library (see below) except views::all, ranges::take_view and ranges::drop_view are replaced with auto(x) since C++23.

11
std::reference_wrapper
https://en.cppreference.com/w/cpp/utility/functional/reference_wrapper

23+
std::basic_common_reference
https://en.cppreference.com/w/cpp/utility/functional/reference_wrapper/basic_common_reference


remove_reference<U&&> ===> U
remove_reference<U&>  ===> U
20
remove_cvref


In fact, std::make_pair() decays the passed argu-
ment (passing them by value in C++03 and using traits since C++11).
Josuttis 105


Using Deduction Guides to Force Decay
Josuttis 107


As usual for make... functions,its arguments decay:
Josuttis 141
*/



