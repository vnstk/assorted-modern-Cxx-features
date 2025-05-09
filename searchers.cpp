/*   (c)2024 Vainstein K.   */
#include "Josuttis--tracknew.hpp"
#include "common.h"
#include <algorithm>
#include <array>
#include <forward_list>
#include <vector>

constexpr std::array<float,7> arrfA{ 1.3, 2.7, 9.1, 2.2, 8.5, 7.4, 6.0, };
// arrfB is just arrfA, sorted ASC.
constexpr std::array<float,7> arrfB{ 1.3, 2.2, 2.7, 6.0, 7.4, 8.5, 9.1, };

const std::forward_list<float> lisfC{ 2.2, 2.7, 9.1, };
const std::forward_list<float> lisfD{ 4.2, 4.7, };
//
using slif_cit_t = decltype(lisfC)::const_iterator;


/*	std::includes fairly straightforw;
		o	assumes both haystack and needle are sorted.
		o	needle, qua subseq, needn't be contiguous.
*/
void test__includes_subsequence ()
{	PRenteredFU;
	decltype(arrfB)::const_iterator haystIt_beg = arrfB.cbegin(),
	                                haystIt_end = arrfB.cend();

	slif_cit_t needleIt00 = lisfC.cbegin(), needleIt01 = lisfC.cbegin();

	std::advance(needleIt01,1U); // Look for { 2.2 }
	SAYevalCHKretBOOL( std::includes(haystIt_beg,haystIt_end , needleIt00,needleIt01)   ,true);

	std::advance(needleIt01,1U); // Look for { 2.2 , 2.7 }
	SAYevalCHKretBOOL( std::includes(haystIt_beg,haystIt_end , needleIt00,needleIt01)   ,true);

	std::advance(needleIt01,1U); // Look for { 2.2 , 2.7 , 9.1 } --- Skipped 3 elems.
	SAYevalCHKretBOOL( std::includes(haystIt_beg,haystIt_end , needleIt00,needleIt01)   ,true);

	slif_cit_t needleIt02 = lisfD.cbegin(), needleIt03 = lisfD.cbegin();
	//
	std::advance(needleIt03, 1U); // Look for { 4.2 }
	SAYevalCHKretBOOL( std::includes(haystIt_beg,haystIt_end , needleIt02,needleIt03)   ,false);
}


const std::vector<float> vecfB{ 1.3, 2.2, 2.7, 6.0, 7.4, 8.5, 9.1, };
const std::vector<float> vecfC{ 7.4, 8.5, };
const std::vector<float> vecfD{ 8.5, 7.4, };
//
static const decltype(vecfB)::const_iterator haystIt_beg = vecfB.cbegin(),
	                                haystIt_end = vecfB.cend();
static const decltype(vecfC)::const_iterator needleIt00 = vecfC.cbegin(),
	                               needleIt01 = vecfC.cend();
static const decltype(vecfD)::const_iterator needleIt02 = vecfD.cbegin(),
	                               needleIt03 = vecfD.cend();

void test__search_substring__dfltSearcher ()
{	PRenteredFU;
#if VER_ge14 // Technically std::search isn't until 17, but GCC has it (the
             // default_searcher variety, leastways) with --std=c++14
	decltype(vecfB)::const_iterator shouldFindYes =
		std::search(haystIt_beg,haystIt_end , needleIt00,needleIt01);
	SAYevalCHKretBOOL( shouldFindYes==haystIt_beg   ,false);
	SAYevalCHKretBOOL( shouldFindYes==haystIt_end   ,false);
	//
	decltype(vecfB)::const_iterator shouldFindNot =
		std::search(haystIt_beg,haystIt_end , needleIt02,needleIt03);
	SAYevalCHKretBOOL( shouldFindNot==haystIt_end   ,true);
#endif
}


void test__search_substring__fancySearchers ()
{	PRenteredFU;
#if VER_ge17
	const std::boyer_moore_searcher srchrBM_shouldFindYes(needleIt00,needleIt01);
	const std::boyer_moore_searcher srchrBM_shouldFindNot(needleIt02,needleIt03);
	//
	{	decltype(vecfB)::const_iterator shouldFindYes =
			std::search(haystIt_beg,haystIt_end , srchrBM_shouldFindYes);
		SAYevalCHKretBOOL( shouldFindYes==haystIt_beg   ,false);
		SAYevalCHKretBOOL( shouldFindYes==haystIt_end   ,false);
		//
		decltype(vecfB)::const_iterator shouldFindNot =
			std::search(haystIt_beg,haystIt_end , srchrBM_shouldFindNot);
		SAYevalCHKretBOOL( shouldFindNot==haystIt_end   ,true);
	}

	const std::boyer_moore_horspool_searcher srchrBMH_shouldFindYes(needleIt00,needleIt01);
	const std::boyer_moore_horspool_searcher srchrBMH_shouldFindNot(needleIt02,needleIt03);
	//
	{	decltype(vecfB)::const_iterator shouldFindYes =
			std::search(haystIt_beg,haystIt_end , srchrBMH_shouldFindYes);
		SAYevalCHKretBOOL( shouldFindYes==haystIt_beg   ,false);
		SAYevalCHKretBOOL( shouldFindYes==haystIt_end   ,false);
		//
		decltype(vecfB)::const_iterator shouldFindNot =
			std::search(haystIt_beg,haystIt_end , srchrBMH_shouldFindNot);
		SAYevalCHKretBOOL( shouldFindNot==haystIt_end   ,true);
	}
#endif
}


int main()
{
	test__includes_subsequence(); // subseq needn't be contiguous.
	test__search_substring__dfltSearcher();
	test__search_substring__fancySearchers();
}
