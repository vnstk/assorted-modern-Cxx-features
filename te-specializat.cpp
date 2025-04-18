/*   (c)2024 Vainstein K.   */
#include "common.h"
#include <array>


template<typename T = float, unsigned N = 5>
struct AllTeParamsHaveDefaults {
	T                _arrSimple[N];
	std::array<T,N>  _arrFancy;
};


template<typename T>
struct Foo {
	T _tee;
	T getty (void) const { return _tee; }
	void printSz (void) const;
	const static unsigned _csu{99U};
	static T _msu;
	static unsigned _zsu;
};
template<typename T> unsigned Foo<T>::_zsu = 66U;
template<typename T> T Foo<T>::_msu = 0;
//
template<typename T>
void Foo<T>::printSz (void) const {
	printf("(Ln%d) Sizeof(T) = %zu\n\t\\___ %s\n",__LINE__, sizeof _tee, currFU);
}

/* Full (explicit) cla te specializat, for 1 ccrete type: must redef entire (sorta) cla.
*/
template<>
struct Foo<bool> {
	bool _boo;
//	bool getty (void) const { return ! _boo; }  //If needed, that is.
	void printSz (void) const;
	const static unsigned _csu{77U};
};
//
template<> float Foo<float>::_msu = 3.14; // Ea specializat has its own static.
//
void Foo<bool>::printSz (void) const {
	fuPRlit("Yeah nah.");
}

void test__full_claTe_specializat ()
{	PRenteredFU;
	Foo<int> fi;
	assert(fi._msu == 0);
	Foo<long> fl_2;
	assert(fl_2._msu == 0);
	fl_2._msu = 33U;
	Foo<long> fl_3;
	assert(fl_3._msu == 33U);
	assert(fi._msu == 0); // Other specializats' static unchanged.
	Foo<bool> fb;
//	assert(fb._msu == 0); // Foo<bool> hadn't bothered to redef _msu, so lacks it.
	Foo<char> fc;
	assert(fc._msu == 0);
	puts("============================================================");
}


/* Partial cla te specializat: say, just for pointer types.
*/
template<typename T>
struct Foo<T*> {
	T *_teeArr[3];
	T *getty (void) const { return ! _teeArr[2]; }
	void printSz (int) const;
};
template<typename T>
void Foo<T*>::printSz (int) const {
	fuPRlit("Yeah ptr nah.");
}

template<typename T, int S>   struct Coll {
	void operator()() {fuPRlit("primary template <T,S>"); }
};
template<int S>               struct Coll<float, S> { 
	void operator()() {fuPRlit("partial specialization <float,S>"); }
};
template<typename T>          struct Coll<T, 10> {
	void operator()() {fuPRlit("partial specialization <T,10>"); }
};
template<typename T, int S>   struct Coll<T*, S> { 
	void operator()() {fuPRlit("partial specialization <T*,S>"); }
};

void test__partial_claTe_specializat ()
{	PRenteredFU;
	Foo<double> food;
	SAYeval( food.printSz() );
	Foo<float> foof;
	SAYeval( foof.printSz() );
	Foo<bool> foob;
	SAYeval( foob.printSz() );
	Foo<long*> foopl;
	SAYeval( foopl.printSz(42) );
	//
	Coll<char, 42> a;// primary template
	SAYeval( a() );
	Coll<float,42> b;// partial specialization <float, S>
	SAYeval( b() );
	Coll<char, 10> c;// partial specialization <T, 10>
	SAYeval( c() );
	Coll<int*, 20> d;// partial specialization <T*, S>
	SAYeval( d() );
#if AMBIG
	Coll<int,   10> e; //ERR, ambig: collection<T,10> or collection<float,S>  ??
	Coll<char*, 10> f; //ERR, ambig: collection<T,10> or collection<T*,S>  ??
#endif
	//
	puts("============================================================");
}



// Partial fu te specializat //Josuttis 356



/* Memb-meth-fu te specializat: just one of the member meths.
*/
	// ref https://www.learncpp.com/cpp-tutorial/class-template-specialization/
template<typename T>
struct Foo_FuTeSpecializ {
	T _tee;
	//
	void printSz (void) const {
		printf("(Ln%d) Sizeof(T) = %zu\n\t//%s//\n",__LINE__, sizeof _tee, currFU);
	}
	//NB: we do *not* declare the bool-specialized printSz(); we just go define it.
};
template<>
void Foo_FuTeSpecializ<bool>::printSz (void) const {
	fuPRlit("Yeah nah, membfunc specialized.");
}

// Standalone-fu te specializat.
template<typename T>
void printSz_standalone (const T& targ) {
	printf("(Ln%d) Sizeof(T) = %zu\n\t//%s//\n",__LINE__, sizeof(targ), currFU);
}
template<>
void printSz_standalone<bool>(const bool& barg) {
	printf("(Ln%d) Yeah nah.  No sizeof(bool) for you.\n",__LINE__);
}
template<>
void printSz_standalone/*<char>*/(const char& barg) {
	//May skip the teargs^^^^^^if cpiler can deduce them from fuparams.
	printf("(Ln%d) Yeah nah.  No sizeof(char) for you.\n",__LINE__);
}

void test__fu_te_specializat ()
{	PRenteredFU;
	Foo_FuTeSpecializ<double> fooFTSd;
	SAYeval( fooFTSd.printSz() );
	Foo_FuTeSpecializ<float> fooFTSf;
	SAYeval( fooFTSf.printSz() );
	Foo_FuTeSpecializ<bool> fooFTSb;
	SAYeval( fooFTSb.printSz() );
	//
	printSz_standalone(3.14f);
	printSz_standalone(314L);
	printSz_standalone(false);
	puts("============================================================");
}


int main() {
	test__full_claTe_specializat();
	test__partial_claTe_specializat();

	test__fu_te_specializat();

	AllTeParamsHaveDefaults<uint16_t,3U> atphdA; // Override all defaults.
	AllTeParamsHaveDefaults<>            atphdB; // Accept all defaults.
}
