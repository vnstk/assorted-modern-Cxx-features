/*   (c)2024 Vainstein K.   */
#include "common.h"
#include <array>
#include <initializer_list>
#include <string>
#include <string_view>
#include <vector>

/* Refresh
terms:
		**rank** is how many dimensions
				static_assert(std::rank<int[][5][7]>{} == 3);
, and
		**extent** is # of elems in a particular dimension
				static_assert(std::extent<int[][5][7], 2>{} == 7);  
*/

#ifdef VER_ge20
#	include <span>

void test__span ()
{	PRenteredFU;
	double seqA[7]; // 1-D plain arr
	double seqB[31][7]; // 2-D plain arr
	double seqC[0][7]; // 2-D plain arr, with ridiclous extent of 1st dim
	double seqD[7][0]; // 2-D plain arr, with ridiclous extent of last dim.

	// Multidimensional array must have bounds for all dimensions except the first
/*
#if 0 // Kinda interesting:
	double seqZ[][7];         <=== won't compile
	double seqZ[][7] = {};    <=== compiles.
	extern double seqZ[][7];  <=== compiles.
#endif
*/
	double seqE[][7] = {};
	extern double seqF[][7];

	double *seqG; // will it work?

	std::array<double,7> seqH;
	std::vector<float> seqI;
	std::vector<float> seqJ(7 /*# of elems to be default-initialized*/);

	std::vector<float> seqP{2.2F,4.4F,6.6F};
	// Note, this is the specially-defd-in-std bool specializat of vec.
	std::vector<bool> seqQ{true,false,true,true,false};
	// Why is a span of initializer_list said to have dynamic extent?  Why isn't initLi.size() constexpr?
	const std::initializer_list<double> seqS{3.3,1.1,2.2,7.7,5.5,4.4,6.6};
	const std::string seqT{"bunnies"};
	const std::string_view seqU{seqT};

	std::span spaS_another{seqS};
	std::span spaY = spaS_another.subspan(1,3);
	std::span spaZ = spaS_another.subspan(1,std::dynamic_extent);

#define ABOUTspan(sp) \
	printf("(Ln%d) \e[32m%6s\e[0m  empty:%c %3zu=size,i.e.#elems "\
			"%2zu=sizeof(T)  %4zu=size_bytes  extent:%s\n",     \
	       __LINE__, #sp, sp.empty()?'Y':'N', sp.size(),   \
			sizeof(decltype(sp)::element_type), sp.size_bytes(),  \
	       (sp.extent==std::dynamic_extent) ? "dyn" : (std::to_string(sp.extent).c_str()))

#define CHKspan(ident)                     \
	std::span spa ## ident{seq ## ident};   \
	ABOUTspan(spa ## ident)

	CHKspan(A);
	CHKspan(B);
//	CHKspan(C);   Multidim array, extent of whose 1st dim is 0.
//	CHKspan(D);   Multidim array, extent of whose last dim is 0.
//	CHKspan(E);   Multidim array, extent of whose 1st dim is unspecified.
//	CHKspan(F);   Multidim extern array, extent of whose 1st dim is unspecified.
//	CHKspan(G);   Just "float*", which of course is same as "float[]".
	CHKspan(H);
	CHKspan(I);
	CHKspan(J);
	CHKspan(P);
//	CHKspan(Q);   The bool specializations of vector; span doesn't like it!
	CHKspan(S);
	CHKspan(T);
	CHKspan(U);
	ABOUTspan(spaY); // On a static-extent subspan?
	ABOUTspan(spaZ); // On a dynamic-extent subspan?
#undef CHKspan
#undef ABOUTspan

	/* Now this is interesting: normally we'd get segfault just
	when initializing charNoneSuch and floatNoneSuch; but here,
	segfault doesn't occur until+unless we *use* those bogus values. */
	char charNoneSuch = spaT[33333];
	float floatNoneSuch = spaP[33333];
if ((uintptr_t)&test__span == 0) // (So that the next line doesn't run.)
	PRmsg("charNoneSuch=%c floatNoneSuch=%f\n", charNoneSuch,floatNoneSuch);
	
}
#endif // VER_ge20
/*
	Turns out, std::span is incompatible with quite a few kinds of sequences!!
*/


#if defined(VER_ge23) || defined(__cpp_lib_mdspan)
#	include <mdspan>
//
#endif

void test__mdspan ()
{	PRenteredFU;
#if defined(VER_ge23) || defined(__cpp_lib_mdspan)
#	include <mdspan>
#endif
}


int main ()
{
#if VER_ge20
	test__span();
#endif
}
