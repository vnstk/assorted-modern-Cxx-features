/*   (c)2025 Vainstein K.   */
#include "common.h"
#if ! VER_ge17
#	error "Everything in this file needs 17+"
#endif
#include <string>
#include <string_view>


#define cstrB "flarpeggio"

void test__sv()
{	PRenteredFU;

	// literals copied from https://en.cppreference.com/w/cpp/string/basic_string/operator_basic_string_view
	std::string narStr = "ラーメン";   // narrow string
	std::wstring widStr = L"ラーメン";  // wide string

	std::string_view   narSVw = narStr.operator std::string_view();
	std::wstring_view  widSVw = widStr.operator std::wstring_view();
	
	// The ungainly way to cstruct a string_view from a string; with a ctor
	std::string_view sv0{narStr.c_str()};
	std::string_view sv1{narStr.c_str(), 5U}; // first 5 codepoints

	static char const *cstrE = "flarpeggio";
	static char const cstrF[] = {'f','l','a','r','p','e','g','g','i','o','\0'};
	// But, that ctor works as well wrt to an actual C string.

	std::string_view sv3{cstrB};
	std::string_view sv4{cstrE};
	std::string_view sv5{cstrF};
	std::string_view sv6{__FILE__};
	PRmsg("sv3[%s] sv4[%s] sv5[%s] sv6[%s]\n",
	      sv3.data(), sv4.data(), sv5.data(), sv6.data());

	// SV.data() might be NULL
	std::string emptyS;
	assert(emptyS.data() != nullptr);
	std::string_view emptySV;
	assert(emptySV.data() == nullptr);

	// SV.data(), if not NULL, might not **end with** NUL.
	std::string const sP{"bunnies"};
	std::string_view sv8 = sP; // implicit cversion
	PRmsg("s.length()=%zu sv.length()=%zu sv.data()=%p sv.empty()=%c\n",
	      sP.length(), sv8.length(), sv8.data(), sv8.empty()?'Y':'N');
	SAYeval( sv8.remove_prefix(7U) );
	PRmsg("sv.length()=%zu sv.data()=%p sv.empty()=%c\n",
	      sv8.length(), sv8.data(), sv8.empty()?'Y':'N');
	// Now empty, but data() still not NULL !
	std::string_view svNada;
	sv8.swap(svNada);
	PRmsg("sv.length()=%zu sv.data()=%p sv.empty()=%c\n",
	      sv8.length(), sv8.data(), sv8.empty()?'Y':'N');

	// A circuitous substitute for strcpy... except that it doesn't NUL-terminate!
	std::string sQ{"flarpeggio"};
	std::string_view sv9 = sQ;
	char buf[32];
	memset(buf, 'Z', sizeof buf);
	buf[sizeof buf - 1] = '\0';
	sv9.copy(buf, /*ct*/ 8, /*pos*/ 2);
	PRmsg("buf=\"%s\"\n", buf);
}


int main () {
	test__sv();
}


#if 0
also: to_chars and from_chars

https://en.cppreference.com/w/cpp/string/basic_string/erase2


size vs ssize, https://en.cppreference.com/w/cpp/iterator/size

char arr lit --> basic_string
https://en.cppreference.com/w/cpp/string/basic_string/operator%22%22s

https://en.cppreference.com/w/cpp/string/char_traits

resize_and_overwrite
(C++23)
changes the number of characters stored and possibly overwrites indeterminate contents via user-provided operation

23
https://en.cppreference.com/w/cpp/string/basic_string/contains


#endif
