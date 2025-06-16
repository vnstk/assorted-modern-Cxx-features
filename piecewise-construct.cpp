/*   (c)2025 Vainstein K.   */
#include "common.h"
#include <tuple>
#include <utility> //For  piecewise_construct_t & std::pair

								// notes, Josuttis, "STL" ed2
/* std::pair's 3 ctors:

A.	pair(const T1& , const T2&);
B.	pair(      T1&&,        2&&);
C.	pair(std::piecewise_construct_t,  tuple<Args1...>,  tuple<Args2...>);

A & B: given 2 tuples, creates pair<1stTuple,2ndTuple>.

C: given 2 tuples, creates pair<objCstructedFrom1stTuple,objCstructedFrom2ndTuple>.

"piecewise" ==> "two pieces"
*/

/* IOW,

  Oload that NOT-uses std::piecewise_construct_t assumes that each tuple arg becomes the elem of a pair.

  Oload that YES-uses std::piecewise_construct_t assumes that each tuple argument is used to construct, piecewise, a new object of specified type; and *that* obj will become the element of the pair.
*/

/* YA explanation:  std::piecewise_construct says " this tuple I'm about to show you?  Pretend it was an initialization_list.  And same for the tuple after it. "
*/

struct Fnord
{
	float _f{1.1F};
	long  _l{22L};
	unsigned _u{33U};

#if 0
	Fnord (std::tuple<float,long,unsigned> tup) 
	:	_f{std::get<float>(tup)}
	,	_l{std::get<long>(tup)}
	,	_u{std::get<unsigned>(tup)}
	{	fuPRlit("Ctor taking a 3-elem tuple");
	}
#endif

	Fnord (float f, long l, unsigned u)
	:	_f{f}
	,	_l{l}
	,	_u{u}
	{	fuPRlit("Ctor taking 3 discrete primitives");
	}

	template<typename... Args>
	Fnord (Args... args)
	{	fuPRlit("Ctor taking param-pack");
	}

};

using FLU_t = std::tuple<float,long,unsigned>;


void test__piecewise_construct()
{	PRenteredFU;

	FLU_t tuA(4.4F,55L,66U);

	std::pair<const char*,Fnord> pX("XX", tuA);
		// the "Fnord (Args... args)" ctor is used.
	PRtyp(pX.first);
	PRtyp(pX.second);

	std::pair<const char*,Fnord> pY( std::piecewise_construct,
	                                std::make_tuple("YY"), tuA);
		// the "Fnord (float f, long l, unsigned u)" ctor is used.
	PRtyp(pY.first);
	PRtyp(pY.second);

#if 0
	auto pA = std::make_pair( std::make_tuple(4.4F,55L,66U) , std::make_tuple(7.7F,88L,99U) );
	PRtyp(pA.first); // ---> FLU_t

	std::pair<FLU_t,FLU_t> pB{ std::make_tuple(4.4F,55L,66U) , std::make_tuple(7.7F,88L,99U) };
	PRtyp(pB.first); // ---> FLU_t
#endif
#if 0   // Won't compile once the  "Fnord (std::tuple<float,long,unsigned>)"  ctor taken away.
	std::pair<Fnord,Fnord> pC{ std::make_tuple(4.4F,55L,66U) , std::make_tuple(7.7F,88L,99U) };
	PRtyp(pC.first); // --> Fnord
#endif

#if 0   // Just because pair ctor has such an oload, doesn't mean make_pair() has one too.
	auto pD = std::make_pair(  std::piecewise_construct ,
	                           std::make_tuple(4.4F,55L,66U) , std::make_tuple(7.7F,88L,99U) );
#endif
#if 0
	auto pZ = std::pair{ std::piecewise_construct ,
	                           std::make_tuple(4.4F,55L,66U) , std::make_tuple(7.7F,88L,99U) };

	std::pair<FLU_t,FLU_t> pE{ std::piecewise_construct ,
	                           std::make_tuple(4.4F,55L,66U) , std::make_tuple(7.7F,88L,99U) };
	PRtyp(pE.first); // ---> FLU_t

	std::pair<Fnord,Fnord> pF{ std::piecewise_construct ,
	                           std::make_tuple(4.4F,55L,66U) , std::make_tuple(7.7F,88L,99U) };
	PRtyp(pF.first); // ---> Fnord
#endif
}

#if 0
std::string stringif_pair_of_Fnord (std::pair<Fnord,Fnord>& pear) {
	char buf[128];
	sprintf(buf, "\n\tfirstFnord:(_f=%.1f _l=%ld _h=%hu}", pear.first._f, pear.first._l, pear.first._h);
	sprintf(buf, "\n\tsecondFnord:(_f=%.1f _l=%ld _h=%hu}", pear.second._f, pear.second._l, pear.second._h);
	return std::string(buf);
}

	std::pair<Fnord,Fnord> p0(3.3F,55L);

	Fnord(3.3F,55L

    std::tuple<float,long,short> tupA(3.3F,44L,55);
    std::tuple<float,long,short> tupB(6.6F,77L,88);

	//========================= Ctor tags ==================================================

	std::tuple<long,float> tuA( 42L,3.14f);
	std::tuple<long,float> tuB(842L,31.4f);
	printf("Without piecewise_construct, each tuple arg will stay itself (i.e. a tuple).\n");
	std::pair<Bar,Bar> pearX(                          tuA,tuB);
	printf("With piecewise_construct, each N-tuple will donate its elems for use as args to a ctor taking N args.\n");
	std::pair<Bar,Bar> pearY(std::piecewise_construct, tuA,tuB);
#endif



int main () {
	test__piecewise_construct();
}
