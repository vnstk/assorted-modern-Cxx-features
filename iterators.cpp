/*   (c)2024 Vainstein K.   */
#include "common.h"
#include <concepts>
#include <iterator>
//
#include <algorithm> // for: iter_swap
#include <array>
#include <deque>
#include <forward_list>
#include <list>
#include <vector>


void test__print_some_properties ()
{	PRenteredFU;

	std::vector<char> co{'b','u','n','n','i','e','s'};
	std::back_insert_iterator<decltype(co)> backInsAdapter = std::back_inserter(co);
	PRbasicITERinfo(backInsAdapter);
	PRextraITERinfo(backInsAdapter);
	decltype(co)::iterator it = co.begin();
	PRbasicITERinfo(it);
	PRextraITERinfo(it);
	decltype(co)::const_iterator cit = co.cbegin();
	PRbasicITERinfo(cit);
	PRextraITERinfo(cit);

	std::array<float,42>       arr;
	std::vector<float>         vec;
	std::deque<float>          deq;
	std::forward_list<float>   singlyLinkedLi;
	std::list<float>           doublyLinkedLi;
	std::string                str{"bunnies"};
	//
	puts("--------------------------------------------------------------");
	PRbasicITERinfo(arr.begin()); PRextraITERinfo(arr.begin());
	puts("--------------------------------------------------------------");
	PRbasicITERinfo(vec.begin()); PRextraITERinfo(vec.begin());
	puts("--------------------------------------------------------------");
	PRbasicITERinfo(deq.begin()); PRextraITERinfo(deq.begin());
	puts("--------------------------------------------------------------");
	PRbasicITERinfo(singlyLinkedLi.begin()); PRextraITERinfo(singlyLinkedLi.begin());
	puts("--------------------------------------------------------------");
	PRbasicITERinfo(doublyLinkedLi.begin()); PRextraITERinfo(doublyLinkedLi.begin());
	puts("--------------------------------------------------------------");
	PRbasicITERinfo(str.begin()); PRextraITERinfo(str.begin());
}


void test__helpers_for_adding_ctainer_elems ()
{	PRenteredFU;
	std::list<float> dest({ 2.2F , 9.9F , 4.4F , 1.1F });

	decltype(dest)::iterator afterTheSecond = std::next(dest.begin(), 2U);

	std::front_insert_iterator<decltype(dest)> willCall__push_front = std::front_inserter(dest);
	std::insert_iterator<decltype(dest)>       willCall__insert     = std::inserter(dest, afterTheSecond);
	std::back_insert_iterator<decltype(dest)>  willCall__push_back  = std::back_inserter(dest);
	
	std::vector<float> copyFrom({ 8.8F , 7.7F , 5.5F });
	std::vector<float>::const_iterator srcBegin = copyFrom.cbegin();

	std::copy(srcBegin+0,srcBegin+1, willCall__insert);
	std::copy(srcBegin+1,srcBegin+2, willCall__push_back);
	std::copy(srcBegin+2,srcBegin+3, willCall__push_front);
	/* Now dest ought be:
									{5.5  2.2  9.9  8.8  4.4  1.1  7.7}        */
	printf("Dumping dest   {");
	for (float const& elem : dest)
		printf("  %.1f", elem);
	printf("  }\n");
}

using listf_t = std::list<float>;
using listf_iter__fwd_notConst_t = listf_t::iterator;
using listf_iter__fwd_yesConst_t = listf_t::const_iterator;
using listf_iter__rev_notConst_t = std::reverse_iterator<listf_iter__fwd_notConst_t>;
using listf_iter__rev_yesConst_t = std::reverse_iterator<listf_iter__fwd_yesConst_t>;

void test__adaptors ()
{	PRenteredFU;
	std::list<float> seq({ 2.2F , 9.9F , 4.4F , 1.1F });

	listf_iter__fwd_notConst_t it00 = seq.begin();
	listf_iter__fwd_yesConst_t it01 = seq.cbegin();
	listf_iter__fwd_notConst_t it02 = seq.end();
	listf_iter__fwd_yesConst_t it03 = seq.cend();

	listf_iter__rev_notConst_t it04 = seq.rbegin();
	listf_iter__rev_yesConst_t it05 = seq.crbegin();
	listf_iter__rev_notConst_t it06 = seq.rend();
	listf_iter__rev_yesConst_t it07 = seq.crend();

	SAYevalCHKretBOOL(*it04 == *std::prev(it02)  ,true);
//	SAYevalCHKretBOOL( it04 ==  std::prev(it02)  ,true);  ERR!  Won't even compile.
	
//	std::move_iterator<decltype(seq)> = std::make_move_iterator(seq);

#if VER_ge14
	listf_iter__rev_notConst_t it08 = std::make_reverse_iterator(it00 /*.begin()*/);
	listf_iter__rev_yesConst_t it09 = std::make_reverse_iterator(it01 /*.cbegin()*/);
	listf_iter__rev_notConst_t it10 = std::make_reverse_iterator(std::prev(it02) /*.end()*/);
	listf_iter__rev_yesConst_t it11 = std::make_reverse_iterator(it03 /*.cend()*/);
#endif
	//TODO  finish this slog
}



template<typename Iter>
using ItCateg = typename std::iterator_traits<Iter>::iterator_category;

template<class Iter>
constexpr const char *iterCategName (void)
{
/* To check the new-lang-feat-of-20 contiguous_iterator, we must make a special 
   effort; for, as https://en.cppreference.com/w/cpp/iterator/iterator_tags says,
		"There is no separate tag for LegacyContiguousIterator. That is, it is not possible to tell a LegacyContiguousIterator based on its iterator_category. To define specialized algorithm for contiguous iterators, use the contiguous_iterator concept.(since C++20)"
*/
	if
#if VER_ge20
	   constexpr (std::contiguous_iterator<Iter>)
#else
	             (false)
#endif
                                                                            return "contiguous";
	else if (std::is_same<ItCateg<Iter>,std::random_access_iterator_tag>()) return "random_access";
	else if (std::is_same<ItCateg<Iter>,std::bidirectional_iterator_tag>()) return "bidirectional";
	else if (std::is_same<ItCateg<Iter>,std::forward_iterator_tag>())       return "forward";
	else if (std::is_same<ItCateg<Iter>,std::output_iterator_tag>())        return "helper for adding ctainer elems";
	else return "Pretty lame!";
}


void test__iter_swap()
{	PRenteredFU;
	std::list<float>           doublyLinkedLi;
	std::vector<float>         vec;

	doublyLinkedLi.push_front(9.9F);
	doublyLinkedLi.push_front(5.5F);
	doublyLinkedLi.push_front(7.7F);

	std::back_insert_iterator<decltype(vec)> vecPushBacker = std::back_inserter(vec);
	PRmsg("vecPushBacker    --> %s\n", iterCategName<decltype(vecPushBacker)>());
	
	std::copy(doublyLinkedLi.cbegin(), doublyLinkedLi.cend(), vecPushBacker);

	decltype(vec)::iterator itFront = vec.begin(),
                            itBack  = std::prev(vec.end(), 1U);
	decltype(vec)::iterator itFront_copy = itFront;
	SAYevalCHKret(vec.front(), "%.1f", 7.7F);
	SAYevalCHKret(*itFront, "%.1f", 7.7F);
	SAYevalCHKret(*itBack , "%.1f", 9.9F);
	std::iter_swap(itBack, itFront); // Swap elems that iters point at.
	SAYevalCHKret(vec.front(), "%.1f", 9.9F);
	SAYevalCHKret(*itFront, "%.1f", 9.9F);
	SAYevalCHKret(*itBack , "%.1f", 7.7F);
	SAYevalCHKretBOOL(itFront==itFront_copy   ,true); // Cfm that iterators themselves still same.
}

int main()
{
	test__print_some_properties();
	test__iter_swap();
	test__adaptors();
	test__helpers_for_adding_ctainer_elems();
}

#if 0
https://en.cppreference.com/w/cpp/iterator
	... sentinel, yet??

counted_iterator
(C++20)
iterator adaptor that tracks the distance to the end of the range

https://en.cppreference.com/w/cpp/iterator/back_inserter
	--- When need this??

indirectly_readable_traits

indirect_result_t
computes the result of invoking a callable object on the result of dereferencing some set of indirectly_readable types
(alias template)


projected
helper template for specifying the constraints on algorithms that accept projections


basic_const_iterator
(C++23)
iterator adaptor that converts an iterator into a constant iterator
#endif
