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


struct KlasU {
	char _ch{'Z'};
	KlasU () { fuPRlit("called yes-dflt ctor."); }
	KlasU (char ch) : _ch(ch) { fuPRlit("called non-dflt ctor."); }
};

void test__invoking_dflt_ctor()
{	PRenteredFU;
	SAYeval( KlasU kc0 );
	SAYeval( KlasU kc1{} );
#if 0
	// Bad idea!!!  Gets disambiguated as a _func_declarat_.
	SAYeval( KlasU kc2() );
#endif
	// Parsed all right, but avoidable copy.
	SAYeval( KlasU kc3 = KlasU() );
}


struct KlasD {
private:
	char _ch{'Z'};
};
struct KlasE {
public:
	char _ch{'Z'};
};
struct KlasF {
private:
	static char _ch;
};
struct KlasG {
public:
	static char _ch;
};

char KlasF::_ch = 'Z';
char KlasG::_ch = 'Z';

// If a class has private non-static datamemb, we need ctor to initialize it. ---Stroustrup
void test__when_ctor_required()
{	PRenteredFU;
	KlasD kd; // Apparently, no we don't. ---reality
	KlasE ke;
	KlasF kf;
	KlasG kg;
}


struct BaseA {
	char _ch;
	BaseA () : _ch('Z') { }
};
struct BaseB {
	char _ch;
	BaseB (char ch) : _ch(ch) {   fuPRmsg("'char' ctor, _ch='%c'\n", _ch);   }
};

struct KlasH {
	char _ch;
	KlasH (char ch) : _ch(ch) {   fuPRmsg("_ch='%c'\n", _ch);   }
	// Deleg to another ctor of same class.
	KlasH (char const *s) : KlasH(s[0]) {   fuPRmsg("_ch='%c'\n", _ch);   }
};
struct KlasH_WithDfltCtor { // Same as KlasH, but with defaulted dflt ctor.
	char _ch;
	KlasH_WithDfltCtor () =default;
	KlasH_WithDfltCtor (char ch) : _ch(ch) {}
	KlasH_WithDfltCtor (char const *s) : KlasH_WithDfltCtor(s[0]) {}
};

struct KlasJ : BaseB {
	// Deleg to ctor of a base class, { ch }
	KlasJ (char ch) : BaseB{ch} {   fuPRlit("done delegating");   }
};
struct KlasK : BaseB {
	// Deleg to ctor of a base class, ( ch )
	KlasK (char ch) : BaseB(ch) {   fuPRlit("done delegating");   }
};

struct KlasP : BaseA { 
//must initializ a base explciitly, unless said base has dflt ctor
};

void test__delegating_ctors()
{	PRenteredFU;
#if 0
	SAYeval(  KlasH kh0                 ); // Nnnnope: lacks dflt ctor.
#endif
	SAYeval(  KlasH_WithDfltCtor kh0    ); // Have dflt ctor, will travel.
	//
	SAYeval(  KlasH kh1{'Z'}            );
	SAYeval(  KlasH kh2{"Zounds!!!"}    );
	SAYeval(  KlasJ kj{'Z'}             );
	SAYeval(  KlasK kk{'Z'}             );
	SAYeval(  KlasP kp                  );
}


struct BaseC {
	BaseC() =default;
public:
	char  _cha{'Z'};
	BaseC (char  x) : _cha(x) { fuPRmsg("_cha=%c _flo=%.2f _sho=%hd\n",_cha,_flo,_sho); }
protected:
	float _flo{3.14};
	BaseC (float x) : _flo(x) { fuPRmsg("_cha=%c _flo=%.2f _sho=%hd\n",_cha,_flo,_sho); }
private:
	short _sho{-42};
	BaseC (short x) : _sho(x) { fuPRmsg("_cha=%c _flo=%.2f _sho=%hd\n",_cha,_flo,_sho); }
};

struct KlasQ : BaseC {
#if 0
	KlasQ (char cha) : _cha(cha) { }
	// Mayn't initializ a base's memb, howsoe'er public be it.
#endif
	KlasQ (char cha) : BaseC(cha) {   fuPRlit("der-class ctor");   }
	//==========================================//
	// protected ctor accessible
	KlasQ (char cha,char) : BaseC(5.67F) {   fuPRlit("der-class ctor");   }
	//==========================================//
#if 0
	// private ctor not accessible
	KlasQ (char cha,char,char) : BaseC((short)-33) {   fuPRlit("der-class ctor");   }
#endif
};

struct KlasM : BaseC {
#if 0
	KlasM (float flo) : _flo(flo) {   fuPRlit("der-class ctor, done.");   }
#endif
	KlasM (float flo) {   _flo=flo;   fuPRlit("der-class ctor, done.");   }
	void f (float flo) { _flo=flo; }
};
struct KlasMM : BaseC {
	using BaseC::_flo;
#if 0
	KlasMM (float flo) : _flo(flo) {   fuPRlit("der-class ctor, done.");   }
#endif
	// We *are* able to mutate _flo from a ctor --- just not through memb init list.
	KlasMM (float flo) {   _flo=flo;   fuPRlit("der-class ctor, done.");   }
	void f (float flo) { _flo=flo; }
};
struct KlasMMM : BaseC {
	using BaseC::BaseC;
	KlasMMM (float flo) : BaseC(flo) {   fuPRlit("der-class ctor, done.");   }
};

struct KlasNN : BaseC {
#if 0   // _sho is so private it's illegal to even mention it!
	using BaseC::_sho;
	    // Needless to say, actually doing anything to _sho is forbidden.
#endif
};
struct KlasNNN : BaseC {
	using BaseC::BaseC;
#if 0   // Alas, the "using" magic doesn't extend to private datamembs of a base.
	KlasNNN (short sho) : BaseC(sho) {   fuPRlit("der-class ctor, done.");   }
	    // Needless to say, actually doing anything to _sho is forbidden.
#endif
};


void test__effect_of_the_using_keyword()
{	PRenteredFU;
	KlasM   km(1.2F);
	KlasMM  kmm(11.2F);
	KlasMMM kmmm(111.2F);
	BaseC& bkm   = km;
	BaseC& bkmm  = kmm;
	BaseC& bkmmm = kmmm;
#if 0
	bkm._flo = 3.4F;
	bkmm._flo = 33.4F;
	bkmmm._flo = 333.4F;

	km._flo = 5.6F;
#endif
	// Behold the glorious achievement of "using" when applied to a non-ctor memb of a base!!
	// Yea, we may mutate it as if 'twere public, though it be protected.
	kmm._flo = 55.6F;
#if 0
	kmmm._flo = 555.6F;
#endif
}



int main(){
	test__effect_of_the_using_keyword();
	test__delegating_ctors();
	test__when_ctor_required();
	test__invoking_dflt_ctor();
	test__EBO();
	test__preferring_inilist_ctor();
	test__brace_initializ_of_POD();
}
