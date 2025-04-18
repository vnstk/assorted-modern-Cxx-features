/*   (c)2024 Vainstein K.   */
#include "common.h"

template<typename T>
struct StruA {
	T _arr[3];
	T const& get1th () const { return _arr[1]; }
};

template<typename T, int N>
struct StruB {
	T _arr[N];
	T const& get1th () const { return _arr[1]; }
};


template<typename T>
T fadd (T x, T y) { return x + y; }

template<typename T1, typename T2, unsigned N>
T1 foo (T2 t2arg, const char *label) {
	printf("[%s]  Nuts! N=%u\n", label, N);
	return (T1) t2arg;
}

template<typename T1, typename T2>
T1 bar (T2 t2arg, const char *label) {
	printf("[%s]  Nuts! \n", label);
	return (T1) t2arg;
}


// Explicit instantiat: **definit**

template struct StruA<float>;
template struct StruA<long>;
template struct StruB<long,5>;


template float fadd (float,float);

template long bar<long,short>(short, const char*);
//
	// In this case, may omit te args:
template double bar(float, const char*);

	// NB: everything concrete and explicit, incl nontyp te args!
template double foo<double,float,42U>(float,const char*);
//
	// In this case, the te args cannot be deduced, hence err
	// when try to omit te args.
//template long foo(short,const char*);		//ERR:w



int main () {
}
