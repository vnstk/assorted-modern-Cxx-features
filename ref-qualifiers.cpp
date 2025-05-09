/*   (c)2024 Vainstein K.   */
#include "common.h"
#include <utility> //For   as_const()


#if VER_ge23 || defined(__cpp_explicit_this_parameter)
struct Klas {
	void f (this const Klas&  selfie, uint8_t u) {//same as   void foo(int i) const &;
	}
	void f (this       Klas&  selfie, uint8_t u) {//same as   void foo(int i) &;   yeah?
	}
	void f (this       Klas&& selfie, uint8_t u) {//same as   void foo(int i) &&;  yeah?
	}
};
//
void test__explicit_this () {
	//TODO, once GCC supports explicit-this
}
#endif



struct Foo {
	void g ( )          { PRlit("  Foo; the   '\e[7m        \e[0m'   oload."); }
	void g ( ) const    { PRlit("  Foo; the   '\e[7mconst   \e[0m'   oload."); }
};

struct Bar {
	void g ( )       &  { PRlit("  Bar; the   '\e[7m      & \e[0m'   oload."); }
	void g ( )       && { PRlit("  Bar; the   '\e[7m      &&\e[0m'   oload."); }
	void g ( ) const &  { PRlit("  Bar; the   '\e[7mconst & \e[0m'   oload."); }
	void g ( ) const && { PRlit("  Bar; the   '\e[7mconst &&\e[0m'   oload."); }
};
struct BarNC {
	void g ( )       &  { PRlit("BarNC; the   '\e[7m      & \e[0m'   oload."); }
	void g ( )       && { PRlit("BarNC; the   '\e[7m      &&\e[0m'   oload."); }
};
struct BarYC {
	void g ( ) const &  { PRlit("BarYC; the   '\e[7mconst & \e[0m'   oload."); }
	void g ( ) const && { PRlit("BarYC; the   '\e[7mconst &&\e[0m'   oload."); }
};

struct QuxL {
	void g ( )       &  { PRlit(" QuxL; the   '\e[7m      & \e[0m'   oload."); }
	void g ( ) const &  { PRlit(" QuxL; the   '\e[7mconst & \e[0m'   oload."); }
};
struct QuxR {
	void g ( )       && { PRlit(" QuxR; the   '\e[7m      &&\e[0m'   oload."); }
	void g ( ) const && { PRlit(" QuxR; the   '\e[7mconst &&\e[0m'   oload."); }
///	void g ( )          { fuPRlit("Hmm!");                }   // ERR!
};


void test__ref_qualif ()
{	PRenteredFU;
#define playWithRefQual(ClassName) {                    \
	printf("\n---------------------------------\n");    \
	ClassName z;                                        \
	SAYeval(   z.g()   );                               \
	SAYeval(   std::move(z).g()   );                    \
	SAYeval(   ClassName().g()    );                    \
}
////
	playWithRefQual(Foo);
	playWithRefQual(Bar);
	playWithRefQual(BarNC);
	playWithRefQual(BarYC);
	playWithRefQual(QuxL);
//	playWithRefQual(QuxR);   // Fine except for the  "z.g();"  part; see below.

	printf("\n---------------------------------\n");
	QuxR z;
//	z.g();    //cpile ERR:     passing 'QuxR' as 'this' argument discards qualifiers
	SAYeval(   std::move(z).g()   );
	SAYeval(   QuxR().g()         );

	printf("\n\e[1m=============================================================\e[0m\n");
//   Wanted also to try invoking g() of a *const object*; but
//   couldn't with BarNC and QuxR, so had to write a separate macro.
//
#define playConst(ClassName)                             \
	printf("\n---------------------------------\n");     \
	const ClassName coz##ClassName;                      \
	SAYeval(   coz##ClassName.g()   )
////
	playConst(Foo);
	playConst(Bar);
//	playConst(BarNC);  //"passing 'const BarNC' as 'this' argument discards qualifiers"
	playConst(BarYC);
	playConst(QuxL);
//	playConst(QuxR);  //"passing 'const QuxR' as 'this' argument discards qualifiers"
}



struct Fnord {
	void g ( )       &  { PRmsg("\tOload '\e[7m      & \e[0m', %s *this\n", figureValueCategoryOfExpr<decltype(*this)>()); }
	void g ( )       && { PRmsg("\tOload '\e[7m      &&\e[0m', %s *this\n", figureValueCategoryOfExpr<decltype(*this)>()); }
	void g ( ) const &  { PRmsg("\tOload '\e[7mconst & \e[0m', %s *this\n", figureValueCategoryOfExpr<decltype(*this)>()); }
	void g ( ) const && { PRmsg("\tOload '\e[7mconst &&\e[0m', %s *this\n", figureValueCategoryOfExpr<decltype(*this)>()); }
};
Fnord mkFnord_a () { return Fnord(); }
Fnord mkFnord_b () { return Fnord{}; }

void test__valueCateg_of_implicit_object()
{	PRenteredFU;
	// This test was an utter bust.  Says *this is an lvalue _in_every_case_.
	Fnord z;
	SAYeval(  z.g()              );
	SAYeval(  std::move(z).g()   );
	SAYeval(  Fnord().g()        );
	SAYeval(  Fnord{}.g()        );
	SAYeval(  (new Fnord)->g()        );
	SAYeval(  mkFnord_a().g()    );
	SAYeval(  mkFnord_b().g()    );
}


void test__as_const() // Not using ref qualifiers per se, but is vaguely related.
{	PRenteredFU;
	Foo          foo;
	Foo       & rfoo =  foo;
	Foo const   cfoo;
	Foo const &rcfoo = cfoo;
	SAYeval(  rcfoo.g()  ); // Selects the const oload.
	SAYeval(   rfoo.g()  ); // Selects the non-const oload.
	SAYeval(   const_cast<Foo const&>(rfoo).g()  ); // Force selection of the const oload --- one way...
#if VER_ge17
	SAYeval(   std::as_const(rfoo).g()  ); // ...and with 17+, there's another way!
#endif
}


int main()
{
#if VER_ge23 || defined(__cpp_explicit_this_parameter)
	test__explicit_this();
#endif
	test__ref_qualif();
	test__valueCateg_of_implicit_object();
	test__as_const();
}

/*
C++11 introduced a little known feature called a ref-qualifier that allows us to overload a member function based on whether it is being called on an lvalue or an rvalue implicit object. 
*/
//		https://www.learncpp.com/cpp-tutorial/ref-qualifiers/
//		https://en.cppreference.com/w/cpp/language/function#Function_declaration
//		https://en.cppreference.com/w/cpp/language/member_functions
