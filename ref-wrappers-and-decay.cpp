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
	std::reference_wrapper<T const> rw = std::ref(r);
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
	std::reference_wrapper<T const> rw = std::cref(r);
	rw = std::cref(rw); // idempotent
	rw = std::cref(rw); // idempotent, I said
	// Of the 3 expected-illegal lines below, only 2nd and 3rd actually evinced error:
	std::reference_wrapper<T const> aWrong = std::ref(r); // ref() won't wrap T&-const
//	std::reference_wrapper<T>       bWrong = std::cref(r); // cref() won't produce wrapper-of-T&-nonconst
//	std::reference_wrapper<T>       cWrong = std::cref(rw); // cref() won't produce wrapper-of-T&-nonconst
}

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

int main (){
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

	static_assert( std::is_const<  std::remove_pointer<decltype(pf)>::type >::value );

	static_assert( std::conjunction<  
		std::is_pointer<decltype(pf)>,  
		std::is_const<std::remove_pointer<decltype(pf)>::type> >::value );

#endif

	test__std_ref__and__std_cref();
}





/*
terminology refresh:
decay means.....  T[]  or  T[]&  ==>   T*    ;  Tfunc  or  Tfunc&  ==>  Tfunc*  .


remove_reference<U&&> ===> U
remove_reference<U&>  ===> U

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

20
std::unwrap_reference
	reference_wrapper<T>  ==>  T&
std::unwrap_ref_decay
https://en.cppreference.com/w/cpp/utility/functional/unwrap_reference

23+
std::basic_common_reference
https://en.cppreference.com/w/cpp/utility/functional/reference_wrapper/basic_common_reference

*/



