/*   (c)2025 Vainstein K.   */
#include "common.h"

#include <algorithm>
#include <list>
#include <set>
#include <unordered_set>
#include <vector>

using vectC_t = std::vector<char>;
using listC_t = std::list<char>;
using setC_t = std::set<char>;
using uosetC_t = std::unordered_set<char>;

constexpr int tagWidth = 18;
//
void pr_vect (vectC_t const& cont, const char *tag, int callerLn) {
	printf("(Ln%d)  \e[33;3m%*s\e[0m --- {", callerLn, tagWidth,tag);
	auto const *p = cont.data();
	for (int i = 0; i < cont.capacity(); ++i) {
		auto const elem = *p;
		if (i < cont.size()) { printf("   %c", elem); }
		else                 { printf("   \e[38;41m%c\e[0m", elem); }
		++p;
	}
	printf("  }  // size=%zu capac=%zu.\n", cont.size(), cont.capacity());
}
void pr_list (listC_t const& cont, const char *tag, int callerLn) {
	printf("(Ln%d)  \e[33;3m%*s\e[0m --- {", callerLn, tagWidth,tag);
	for (auto const& elem : cont) { printf("   %c", elem); }
	printf("  }  // size=%zu.\n", cont.size());
}
void pr_set (setC_t const& cont, const char *tag, int callerLn) {
	printf("(Ln%d)  \e[33;3m%*s\e[0m --- {", callerLn, tagWidth,tag);
	for (auto const& elem : cont) { printf("   %c", elem); }
	printf("  }  // size=%zu.\n", cont.size());
}
void pr_uoset (uosetC_t const& cont, const char *tag, int callerLn) {
	printf("(Ln%d)  \e[33;3m%*s\e[0m --- {", callerLn, tagWidth,tag);
	for (auto const& elem : cont) { printf("   %c", elem); }
	printf("  }  // size=%zu bktCt=%zu.\n", cont.size(), cont.bucket_count());
}

#define PRlist(cont,tag) pr_list((cont),(tag),__LINE__)
#define PRvect(cont,tag) pr_vect((cont),(tag),__LINE__)
#define PRset(cont,tag) pr_set((cont),(tag),__LINE__)
#define PRuoset(cont,tag) pr_uoset((cont),(tag),__LINE__)


// ways to delete some elems:

		// std::remove_if() ---  returns a past-the-end iterator for the new end of the range.
		// std::unique()
		// std::copy()

		// std::vector::erase(it); -- Membfu.  Rets iter following the last removed element.

	// std::vector::resize() --- Membfu.
	// std::list::resize() --- Membfu.

	// std::erase(std::vector) --- //C++20.

	// std::erase(std::list) --- //C++20.
	// std::erase_if(std::list) --- //C++20.

	// std::erase_if(std::set) --- //C++20.
//XXX why no erase() ??????

	// std::erase_if(std::unordered_set) --- //C++20.


void test__delete_from_vect ()
{
	PRenteredFU;
#define INITco vectC_t co{'q','w','e','r','t','y','b','u','n','n','i','e','s'}

	/* resize() ---   Only avail for seq ctainers.
	*/
	{	INITco; PRlit("=========== membfu resize ==========");
		PRvect(co,"ini");
		co.resize(9);
		PRvect(co,"aft membfu resize");
	}

	/* Erase-Remove: A call to std::remove() is typically followed by a call to
	container's erase() membfu to actually remove elements from the container.
		o	std::remove() ---  returns a past-the-end iterator for the new end of the range.
	*/
	{	INITco; PRlit("============= Erase-Remove ============");
		vectC_t::iterator endNowIs =
			std::remove_if(co.begin(), co.end(), [](char elem){return elem > 'm';});
		PRvect(co,"aft std::remove_if");
		SAYevalPRret(  std::distance(co.begin(), endNowIs)   ,"%zu");
		SAYevalPRret(  std::distance(endNowIs, co.end())     ,"%zu");
		vectC_t::iterator followsLastRemovedElem =
			co.erase(endNowIs, co.end());
		PRbasicITERinfo(followsLastRemovedElem);
		PRextraITERinfo(followsLastRemovedElem);
		PRvect(co,"aft membfu erase");
		SAYevalPRret(  std::distance(co.begin(), followsLastRemovedElem)     ,"%zu");
		PRmsg("* followsLastRemovedElem = %c\n", *followsLastRemovedElem);
	}

#if VER_ge20
	/* std::erase_if(std::vector) --- //C++20.
	*/
	{	INITco;
		size_t nElemsErased =
			std::erase_if(co, [](char elem) noexcept {return elem > 'm';});
		// WARNs if -Wnoexcept, and this    ^^^^^^^^ is missing.
		PRvect(co,"aft std::erase_if");
		PRmsg("Erased %zu elems.\n", nElemsErased);
	}
#endif

	/* vector::erase(itA,itB), membfu.  Rets iter to what follows the last removed element.
	*/
	{	INITco; PRlit("=========== membfu erase(); erasing in middle =======");
		vectC_t::iterator begPlus3 = std::next(co.begin(), 3);
		vectC_t::iterator begPlus5 = std::next(begPlus3  , 2);
		vectC_t::iterator endNowIs =
			co.erase(begPlus3, begPlus5); // Rets iter following the last removed element.
		PRvect(co,"aft membfu erase");
		SAYevalPRret(  std::distance(co.begin(), endNowIs)   ,"%zu");
	}

#undef INITco
}


int main()
{
#if defined(__cpp_impl_coroutine)
PRlit("yeah __cpp_impl_coroutine");
#endif
#if defined(__cpp_lib_coroutine)
PRlit("yeah __cpp_lib_coroutine");
#endif

	PRmsg("ver %ld\n",  __cplusplus );
return 0;

	test__delete_from_vect();

}

