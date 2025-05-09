/*   (c)2024 Vainstein K.   */
#include "common.h"
#include "simple-timer.h"
#include <algorithm>
#include <list>
#include <numeric> // Part of the algorithms lib is in <numeric> and not in <algorithm>.
#include <string>
#include <vector>
#include <execution>



using vectC_t = std::vector<char>;

void pr_vectChar (vectC_t const& cont, const char *tag, int callerLn) {
    printf("(Ln%d)  \e[33;3m%18s\e[0m --- {", callerLn, tag);
    for (int i = 0; i < cont.size(); ++i) {
		printf("   %c", cont[i]);
    }
    printf("  }  // sz=%zu cap=%zu.\n", cont.size(), cont.capacity());
}
#define PR_vectChar(cont,tag) pr_vectChar((cont),(tag),__LINE__)
#define INITco vectC_t co{'q','w','e','r','t','y','b','u','n','n','i','e','s'}


struct NumStr {
	/* If ivars were const, std::accumulate wouldn't work; apparently it uses the copy
	   assignm op.  Default-initializes a temporary and *then* assigns into it, I guess.
	   Why not just wait until have the args for non-dflt ctor??
	*/
	unsigned    _num;
	std::string _str;
	NumStr (unsigned num, std::string&& str) noexcept
		: _num(num), _str(std::move(str)) {}
	NumStr (std::string const& str) noexcept : _num(0U), _str(str) {}
};
NumStr foobarize (NumStr const& a, NumStr const& b) {
	char buf[128];
	sprintf(buf, "%s%u|%u%s", a._str.c_str(), a._num, b._num, b._str.c_str());
	return NumStr(a._num * b._num, std::string(buf));
}

void pr_vectNumStr (std::vector<NumStr> const& vec, const char *tag, int callerLn) {
	std::string s{"{"};
	s.reserve(vec.size() * 4 + 2);
	char buf[32];
	for (NumStr const& elem : vec) {
		sprintf(buf, " %s%02u", elem._str.c_str(), elem._num);
		s += buf;
	}
	s += " }";
    printf("(Ln%d)  \e[33;3m%18s\e[0m --- %s\n", callerLn, tag, s.c_str());
}
#define PR_vectNumStr(cont,tag) pr_vectNumStr((cont),(tag),__LINE__)

struct RecordVisitOrder {
	unsigned _counter{0U};
	void operator() (NumStr& x) noexcept { x._num = ++_counter; }
	// Making it noexcept didn't remove the need to enable exceptions so as
	// to catch bad_alloc from an exception_policy-using oload.
};


struct CatAdder {
	std::string _s;
	explicit CatAdder (std::string&& s) : _s(std::move(s)) {}
	float operator()(unsigned& u) {
		(_s += "|") += std::to_string(u);
		u += 10;
		return u * 1.1F;
	}
};


void pr_vectStr (std::vector<std::string> const& vec, const char *tag, int callerLn) {
	std::string s;
	s.reserve(vec.size() * 10 + 2);
	char buf[32];
	for (std::string const& elem : vec) {
		sprintf(buf, "  '%s'", elem.c_str());
		s += buf;
	}
    printf("(Ln%d)  \e[33;3m%18s\e[0m --- {%s  }  sz=%zu cap=%zu\n",
	       callerLn, tag, s.c_str(), vec.size(), vec.capacity());
}

/*
	o	for_each discards retvals of UnaryOp.
	o	transform stores each UnaryOp retval into corresponding output slot.
	o	transform_reduce hands the UnaryOp retvals to the BinaryOps of the "reduce" paert.
*/

void test__for_each ()
{	PRenteredFU;
#if 0
	// Generated with           echo {'"'{a..z}'"'},
	std::vector<NumStr> ini_vec{ {"a"}, {"b"}, {"c"}, {"d"}, {"e"}, {"f"}, {"g"}, {"h"}, {"i"}, {"j"}, {"k"}, {"l"}, {"m"}, {"n"}, {"o"}, {"p"}, {"q"}, {"r"}, {"s"}, {"t"}, {"u"}, {"v"}, {"w"}, {"x"}, {"y"}, {"z"}, };
#endif
	// Generated with           for i in {0..99}; do echo -n '{"x"}, '; done
	std::vector<NumStr> ini_vec{ {"x"}, {"x"}, {"x"}, {"x"}, {"x"}, {"x"}, {"x"}, {"x"}, {"x"}, {"x"}, {"x"}, {"x"}, {"x"}, {"x"}, {"x"}, {"x"}, {"x"}, {"x"}, {"x"}, {"x"}, {"x"}, {"x"}, {"x"}, {"x"}, {"x"}, {"x"}, {"x"}, {"x"}, {"x"}, {"x"}, {"x"}, {"x"}, {"x"}, {"x"}, {"x"}, {"x"}, {"x"}, {"x"}, {"x"}, {"x"}, {"x"}, {"x"}, {"x"}, {"x"}, {"x"}, {"x"}, {"x"}, {"x"}, {"x"}, {"x"}, {"x"}, {"x"}, {"x"}, {"x"}, {"x"}, {"x"}, {"x"}, {"x"}, {"x"}, {"x"}, {"x"}, {"x"}, {"x"}, {"x"}, {"x"}, {"x"}, {"x"}, {"x"}, {"x"}, {"x"}, {"x"}, {"x"}, {"x"}, {"x"}, {"x"}, {"x"}, {"x"}, {"x"}, {"x"}, {"x"}, {"x"}, {"x"}, {"x"}, {"x"}, {"x"}, {"x"}, {"x"}, {"x"}, {"x"}, {"x"}, {"x"}, {"x"}, {"x"}, {"x"}, {"x"}, {"x"}, {"x"}, {"x"}, {"x"}, {"x"}, };

	std::vector<NumStr> vec0{ini_vec};
	std::for_each(vec0.begin(), vec0.end(), RecordVisitOrder{});
	// Expect evidence of visits done in any old order, seq 1 through 150.
	PR_vectNumStr(vec0, "after for_each w/o an execution_policy");

	std::vector<NumStr> vec1{ini_vec};
	unsigned localCounter{0U};
	std::for_each(vec1.begin(), vec1.end(), [&localCounter](NumStr& x){x._num = ++localCounter; });
	// Expect evidence of visits done in any old order, seq 1 through 150.
	PR_vectNumStr(vec0, "after for_each w/o an execution_policy");

#if 0
//If enable this section, will need to compile with    -fexceptions

	Timer t{"for_each with par execution_policy"};
	std::vector<NumStr> vec1{ini_vec};
	std::for_each(std::execution::par, vec1.begin(), vec1.end(), RecordVisitOrder{});
	// Expect evidence of visits done in any old order, seq 1 through M and 1 through N; where M+N=26.
	PR_vectNumStr(vec0, "after for_each with the par execution_policy");
#endif

	// Well it *should've*.
#if 0
//
	std::vector<unsigned> vuX{5,7,9};
	std::for_each(vuX.begin(), vuX.end(), [](unsigned& u){ u+=10U; });
	assert(19 == vuX[2]);
	//
	std::vector<unsigned> vuY{5,7,9};
	CatAdder caPre{"Z"};
	CatAdder caPost = std::for_each(vuY.begin(), vuY.end(), caPre);
	assert(19 == vuY[2]);
	assert(caPost._s == "Z|5|7|9");
	//
	std::vector<unsigned> vuZ{5,7,9,1111};
#if VER_ge17
	decltype(vuZ)::const_iterator citZ = std::for_each_n(vuZ.begin(), 3U,
	                                         [](unsigned& u){ u+=10U; });
	assert(1111U == *citZ);
	assert(19 == vuZ[2]);
#endif
#endif
}

void test__accumulate ()
{	PRenteredFU;
//
	// Oload not taking a dyadic callable, just does addition.
	std::vector<unsigned> vn{5,7,9};
	unsigned sum = std::accumulate(vn.cbegin(), vn.cend(), 20U);
	assert(/*5 + 7 + 9 + 20*/41 == sum);
	//
	// Additions means operator+(), which of course could be oloaded for elem type.
	std::vector<std::string> vs{"aa","bb","cc"};
	std::string cat = std::accumulate(vs.cbegin(), vs.cend(), std::string("dd"));
	PRlit(cat.c_str());
	assert(std::string("ddaabbcc") == cat);
//
	std::vector<NumStr> vns{ {5,"aa"} , {7,"bb"} , {9,"cc"} };
	const NumStr total = std::accumulate(vns.cbegin(), vns.cend(), NumStr{3,"dd"} ,foobarize);
	PRmsg("total _num=%u _str=\"%s\"\n", total._num, total._str.c_str());
	// Prints  str="dd3|5aa15|7bb105|9cc"
}

void test__reduce () // std::reduce similar to std::accumulate, except out of order
{	PRenteredFU;
#ifdef VER_ge17
	std::vector<unsigned> vu{5,7,9};
	unsigned sum00 = std::reduce(vu.cbegin(), vu.cend());
	assert(21U == sum00);

	unsigned sum01 = std::reduce(vu.cbegin(), vu.cend(), 20U);
	assert(41U == sum01);

	std::vector<std::string> vs{"aa","bb","cc"};
	std::string cat = std::reduce(vs.cbegin(), vs.cend(), std::string("dd"));
	PRlit(cat.c_str());
	assert(std::string("ddaabbcc") == cat);

	std::vector<NumStr> vns{ {5,"aa"} , {7,"bb"} , {9,"cc"} };
	const NumStr total = std::reduce(vns.cbegin(), vns.cend(), NumStr{3,"dd"} ,foobarize);
	PRmsg("total _num=%u _str=\"%s\"\n", total._num, total._str.c_str());
	// Prints  str="dd3|5aa15|7bb105|9cc"
#endif
}

void test__transform ()
{	PRenteredFU;
//
	std::vector<unsigned> vu{5,7,9};
	std::list<unsigned> liu{103, 1003, 10003, 100003};
	decltype(liu)::const_iterator third = std::next(liu.cbegin() ,2U);
	assert(10003 == *third);
	decltype(liu)::const_iterator firstNontransformed =
		std::transform(vu.cbegin(), vu.cend(), liu.begin(), [](unsigned u){ return u+10; });
	assert(100003 == *firstNontransformed);
	assert(10003 != *third);
	assert(19    == *third);
	assert(9 == vu[2]); // Confirm that input range is untouched --- unlike with for_each.
}


void test__clusive_scan ()
{	PRenteredFU;
	std::vector<std::string> const startFrom{"aa","bb","cc","dd",};
	constexpr size_t outpSz = 6U; // How many empty strings outp will be initially.
#if VER_ge17
	const auto catenator = [](std::string const& sa,std::string const& sb){ return sa+sb; };

	std::vector<std::string> inp0{startFrom};
	pr_vectStr(inp0, "inp0", __LINE__);
	//NB: XXX std::inclusive_scan expects the output elems to already exist; just reserving capacity isn't enough.
#if 0
	std::vector<std::string> outp0;
	outp0.reserve(inp0.size() * 4);
#endif
	std::vector<std::string> outp0(outpSz);
	decltype(outp0)::iterator ret0 = std::inclusive_scan(
	             inp0.cbegin(), inp0.cend(), outp0.begin(), catenator);
	pr_vectStr(outp0, "outp0,aft", __LINE__);
	PRmsg("inclusive_scan, no initVal, distance(outp.begin,ret) = %ld\n\n",
	             (long) std::distance(outp0.begin(), ret0));
	// Expect:     {  'aa'  'aabb'  'aabbcc'  'aabbccdd'  }

	std::vector<std::string> inp1{startFrom};
	pr_vectStr(inp1, "inp1", __LINE__);
	std::vector<std::string> outp1(outpSz);
	decltype(outp1)::iterator ret1 = std::inclusive_scan(
	             inp1.cbegin(), inp1.cend(), outp1.begin(), catenator, std::string("ee"));
	pr_vectStr(outp1, "outp1,aft", __LINE__);
	PRmsg("inclusive_scan, initVal='ee', distance(outp.begin,ret) = %ld\n\n",
	             (long) std::distance(outp1.begin(), ret1));
	// Expect:     {  'eeaa'  'eeaabb'  'eeaabbcc'  'eeaabbccdd'  }

	std::vector<std::string> inp2{startFrom};
	pr_vectStr(inp2, "inp2", __LINE__);
	std::vector<std::string> outp2(outpSz);
	decltype(outp2)::iterator ret2 = std::exclusive_scan(
	             inp2.cbegin(), inp2.cend(), outp2.begin(), std::string("ee"));
	pr_vectStr(outp2, "outp2,aft", __LINE__);
	PRmsg("exclusive_scan, initVal='ee', default BinaryOp, distance(outp.begin,ret) = %ld\n\n",
	             (long) std::distance(outp2.begin(), ret2));
	// Expect:     {  'ee'  'eeaa'  'eeaabb'  'eeaabbcc'  }

	std::vector<std::string> inp3{startFrom};
	pr_vectStr(inp3, "inp3", __LINE__);
	std::vector<std::string> outp3(outpSz);
	decltype(outp3)::iterator ret3 = std::exclusive_scan(
	             inp3.cbegin(), inp3.cend(), outp3.begin(), std::string("ee"), catenator);
	pr_vectStr(outp3, "outp3,aft", __LINE__);
	PRmsg("exclusive_scan, initVal='ee', distance(outp.begin,ret) = %ld\n\n",
	             (long) std::distance(outp3.begin(), ret3));
	// Expect:     {  'ee'  'eeaa'  'eeaabb'  'eeaabbcc'  }

	// Indeed, can even dispense with separate outp range: just overwrite the input!
	std::vector<std::string> inp4{startFrom};
	pr_vectStr(inp4, "inp4", __LINE__);
	decltype(inp4)::iterator ret4 = std::exclusive_scan(
	             inp4.cbegin(), inp4.cend(), inp4.begin(), std::string("ee"), catenator);
	pr_vectStr(inp4, "inp4,aft", __LINE__);
	PRmsg("exclusive_scan, initVal='ee', overwrite inp, distance(outp.begin,ret) = %ld\n\n",
	             (long) std::distance(inp4.begin(), ret4));
	// Expect:     {  'ee'  'eeaa'  'eeaabb'  'eeaabbcc'  }
#endif
}


void test__transform_and_then_X ()
{	PRenteredFU;
/*
	TODO: transform_inclusive_scan
	TODO: transform_exclusive_scan
	TODO: transform_reduce
*/
}


int main () {
	test__for_each();
	test__clusive_scan();
	test__accumulate();
	test__reduce();//TODO: test with other ExecutionPolicy ??
	test__transform();
}
