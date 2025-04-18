/*   (c)2024 Vainstein K.   */
#include "common.h"
#include <array>
#include <initializer_list>
#include <string>
#include <vector>

#ifdef VER_ge20
#	include <span>
#endif

#if defined(VER_ge23) || defined(__cpp_lib_mdspan)
#	include <mdspan>
//
#endif

void test__span ()
{	PRenteredFU;

	float c_arr_1d[7];
	float c_arr_2d[31][7];
	std::array<float,7> arr00;
	std::vector<float> vec00;
	std::vector<float> vec01(7);
	const std::vector<float> vec02(7);
	const std::vector<float> vec03{3.3F,1.1F,2.2F,7.7F,5.5F,4.4F,6.6F};
	std::vector<bool> vec04{true,false,true,true,false};
	const std::initializer_list<float> ili{3.3F,1.1F,2.2F,7.7F,5.5F,4.4F,6.6F};
	const std::string str00{"bunnies"};

#ifdef VER_ge20

#define ABOUTspan(sp) \
	printf("(Ln%d)  " #sp " --- %3zu=size %4zu=size_bytes empty:%c extent=%s\n",     \
	       __LINE__, sp.size(), sp.size_bytes(), sp.empty()?'Y':'N',         \
	       (sp.extent==std::dynamic_extent) ? "dyn" : (std::to_string(sp.extent).c_str()))

	std::span sp00(c_arr_1d);		ABOUTspan(sp00);
	std::span sp01(c_arr_2d);		ABOUTspan(sp01);
	std::span sp02(arr00);			ABOUTspan(sp02);
	std::span sp03(vec00);			ABOUTspan(sp03);
	vec00.insert(/*before pos*/ vec00.cend(), {11.1F,99.9F,0.003F});
	                      			ABOUTspan(sp03); // Answers *not* updated!!
	std::span sp03b(vec00);			ABOUTspan(sp03b);
	std::span sp04(vec01);			ABOUTspan(sp04);
	std::span sp05(vec02);			ABOUTspan(sp05);
	std::span sp06(vec03);			ABOUTspan(sp05);
	std::span sp07(ili);			ABOUTspan(sp06);
	std::span sp08(str00);			ABOUTspan(sp07);
#endif
}

void test__mdspan ()
{	PRenteredFU;
#if defined(VER_ge23) || defined(__cpp_lib_mdspan)
#	include <mdspan>
#endif
}


int main ()
{
	test__span();
}

#if 0
span
(C++20)
a non-owning view over a contiguous sequence of objects
(class template)

mdspan
(C++23)
a multi-dimensional non-owning array view

#endif
