/*   (c)2025 Vainstein K.   */
#include "common.h"
#include <initializer_list>


struct Foo {
	float  _a;
	float _b;
	float  _c;
	Foo () {
		fuPRmsg("_a=%.3f _b=%.3f _c=%.3f\n",_a,_b,_c);
	}
	Foo (float a, float c) : _a(a), _c(c) {
		fuPRmsg("_a=%.3f _b=%.3f _c=%.3f\n",_a,_b,_c);
	}
	Foo (std::initializer_list<float> ilist) {
		_c = *ilist.begin();
		//NB: _a and _b are _not_ zeroed, even though this is
		// the special so-wonderful Uniform Initialization.
		fuPRmsg("_a=%.3f _b=%.3f _c=%.3f\n",_a,_b,_c);
	}
};

void test__preferring_inilist_ctor()
{	PRenteredFU;
	// x0A's ivars are zeroed, but x0B's are not.  That is exactly
	// contrary to promises of Uniform Initialization.
	PRlit("Foo x0A");
	       Foo x0A  ;
	PRlit("Foo x0B{}");
	       Foo x0B{}  ;
	PRlit("Foo x1(3.3F, 7.7F)");
	       Foo x1(3.3F, 7.7F)  ;
	PRlit("Foo x2{3.3F}");
	       Foo x2{3.3F}  ;
	// x3 will use the inilist ctor, even though the other ctor is better.
	PRlit("Foo x3{3.3F, 7.7F}");
	       Foo x3{3.3F, 7.7F}  ;
	PRlit("Foo x4{3.3F, 5.5F, 7.7F}");
	       Foo x4{3.3F, 5.5F, 7.7F}  ;
	PRlit("Foo x5{3.3F, 5.5F, 7.7F, 9.9F}");
	       Foo x5{3.3F, 5.5F, 7.7F, 9.9F}  ;
	PRlit("Foo x6 = {3.3F}");
	       Foo x6 = {3.3F}  ;
	PRlit("Foo x7 = {3.3F, 7.7F}");
	       Foo x7 = {3.3F, 7.7F}  ;
	PRlit("Foo x8 = {3.3F, 7.7F, 9.9F}");
	       Foo x8 = {3.3F, 7.7F, 9.9F}  ;
	PRlit("Foo x9 = {3.3F, 5.5F, 7.7F, 9.9F}");
	       Foo x9 = {3.3F, 5.5F, 7.7F, 9.9F}  ;
	// Moral: using the () notation ensures you get the benefit of a normal
	// ctor --- and not a ctor taking inilist.
}


struct Bar {
	float  _a;
	float _b;
	float  _c;
};

void test__brace_initializ_of_POD()
{	PRenteredFU;
	// y0's and y1's and y2's ivars are all zeroed.  Go figure.
	//
	PRlit("Bar y0");
	       Bar y0  ;
	PRmsg("y0 --->  _a=%.3f _b=%.3f _c=%.3f\n",y0._a,y0._b,y0._c);
	//
	PRlit("Bar y1{}");
	       Bar y1{}  ;
	PRmsg("y1 --->  _a=%.3f _b=%.3f _c=%.3f\n",y1._a,y1._b,y1._c);
	//
	PRlit("Bar y2={}");
	       Bar y2={}  ;
	PRmsg("y2 --->  _a=%.3f _b=%.3f _c=%.3f\n",y2._a,y2._b,y2._c);
}


struct Empty {
};
struct MerelyQux {
	int64_t _q;
};
struct QuxA : Empty {
	int64_t _q;
};
struct QuxB {
	int64_t _q;
	Empty   _e;
};
struct QuxC {
	Empty   _e;
	int64_t _q;
};

void test__EBO() // Empty Base Optimizat
{	PRenteredFU;
	SAYevalPRret( sizeof(Empty) ,"%zu" );
	SAYevalPRret( sizeof(MerelyQux) ,"%zu" );
	SAYevalPRret( sizeof(QuxA) ,"%zu" ); 
	SAYevalPRret( sizeof(QuxB) ,"%zu" );
	SAYevalPRret( sizeof(QuxC) ,"%zu" );
	SAYevalCHKretBOOL( sizeof(MerelyQux)==sizeof(QuxA) ,true);
};

int main(){
	test__EBO();
	test__preferring_inilist_ctor();
	test__brace_initializ_of_POD();
}
