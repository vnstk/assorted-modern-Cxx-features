/*   (c)2024 Vainstein K.   */
#ifndef COMMON__H
#define COMMON__H

#if __cplusplus == 201103L
#	define VER_eq11 1
#endif
#if __cplusplus >= 201103L
#	define VER_ge11 1
#endif
//
#if __cplusplus == 201402L
#	define VER_eq14 1
#endif
#if __cplusplus >= 201402L
#	define VER_ge14 1
#endif
//
#if __cplusplus == 201703L
#	define VER_eq17 1
#endif
#if __cplusplus >= 201703L
#	define VER_ge17 1
#endif
//
#if __cplusplus == 202002L
#	define VER_eq20 1
#endif
#if __cplusplus >= 202002L
#	define VER_ge20 1
#endif
//
#if __cplusplus == 202302L
#	define VER_eq23 1
#endif
#if __cplusplus >= 202302L
#	define VER_ge23 1
#endif

#if __STDC_VERSION__ == 199901L
#	define cVER_eq99 1
#endif
#if __STDC_VERSION__ >= 199901L
#	define cVER_ge99 1
#endif
//
#if __STDC_VERSION__ == 201112L
#	define cVER_eq11 1
#endif
#if __STDC_VERSION__ >= 201112L
#	define cVER_ge11 1
#endif
//
// C17 introduces no new lang features.
#if __STDC_VERSION__ == 201710L
#	define cVER_eq17 1
#	define cVER_eq23 1  //XXX for now!!
#endif
#if __STDC_VERSION__ >= 201710L
#	define cVER_ge17 1
#	define cVER_ge23 1  //XXX for now!!
#endif


#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <assert.h>


// Quite as Stevens says, bzero(3) is a simplification too good to discard. 
#define bzeroARRAY(x) memset((x), '\0', sizeof (x))
#define bzeroONE(x) memset(&(x), '\0', sizeof (x))


// // // Elementary macros to aid concision.

#define PRlit(lit)    \
	printf("\t/Ln%d/  %s\n",__LINE__, (lit))
#define PRtxt(...)    \
	printf("\t/Ln%d/  %s\n",__LINE__, #__VA_ARGS__)
//
#define PRmsg(printfFmt,...)        \
	printf("/Ln%d/  " printfFmt, __LINE__, __VA_ARGS__)
//
#define PRval(z,printfFmt) \
	printf("/Ln%d/  Value of \"\e[36m" #z "\e[0m\" is \"\e[1m" printfFmt "\e[0m\"; sizeof=%zu\n",  __LINE__, (z), sizeof(z))
//
#define PRbool(z) \
	printf("/Ln%d/  Pred \"\e[36m" #z "\e[0m\" is \e[1m%s\e[0m.\n",  __LINE__, (z)?"true":"false")

#define SAYeval(z) \
	printf("/Ln%d/  Shall \"\e[36m" #z "\e[0m\" now.\n",  __LINE__); z
//
#define SAYeval_store(z,whither) \
	printf("/Ln%d/  Shall \e[1m" #whither "\e[0m := \"\e[36m" #z "\e[0m\" now.\n",  __LINE__); whither = z
//
#define SAYevalPRret(z,retPrintfFmt) \
	printf("/Ln%d/  Eval \"\e[36m" #z "\e[0m\" returned \"\e[1m" retPrintfFmt "\e[0m\"\n",  __LINE__, (z))
//
#define SAYevalCHKret(z,retPrintfFmt,expectedRetval) \
	printf("/Ln%d/  Chk that \"\e[36m" #z "\e[0m\" evals to \"\e[1m" retPrintfFmt "\e[0m\"\n",  __LINE__, (expectedRetval)); assert((expectedRetval) == (z))

#define SAYevalCHKretCSTR(z,expectedRetval) \
	printf("/Ln%d/  Chk that \"\e[36m" #z "\e[0m\" evals to \"\e[1m%s\e[0m\"\n",  __LINE__, (expectedRetval)); assert(!strcmp( (z),(expectedRetval)))
//
#define SAYevalCHKretBOOL(z,expectedRetval) \
	printf("/Ln%d/  Chk that \"\e[36m" #z "\e[0m\" evals to \e[1m%s\e[0m\n",  __LINE__, (expectedRetval)?"true":"false"); assert((expectedRetval) == (z))


////// C++-only section --- begin //////
#if VER_ge11

#include <type_traits>
#include <functional> // For std::is_bind_expression
//
#define PRwhat(z) \
	printf("/Ln%d/  Expr \"\e[36m" #z "\e[0m\" is\e[32;3m%s%s%s"\
"%s%s"\
"%s%s%s"\
"%s%s%s%s"\
"%s%s%s%s\e[0m; sizeof=%zu alignof=%llu.\n",  __LINE__   \
, std::is_const<decltype(z)>::value ? " const" : ""   \
, std::is_array<decltype(z)>::value ? " array" : ""   \
, std::is_member_pointer<decltype(z)>::value ? " membptr" : ""   \
\
, std::is_pointer<decltype(z)>::value ? " ptr" : ""   \
, std::conjunction<   \
	std::is_pointer<decltype(z)>,  \
	std::is_const<typename std::remove_pointer<decltype(z)>::type> >::value ? "-to-const" : ""  \
\
, std::is_lvalue_reference<decltype(z)>::value ? " lvalRef" : ""   \
, std::is_rvalue_reference<decltype(z)>::value ? " rvalRef" : ""   \
, std::conjunction<   \
	std::is_reference<decltype(z)>,  \
	std::is_const<typename std::remove_reference<decltype(z)>::type> >::value ? "-to-const" : ""  \
\
, (std::is_enum<decltype(z)>::value ) ? " enum" : ""   \
, (std::is_union<decltype(z)>::value ) ? " union" : ""   \
, (std::is_class<decltype(z)>::value ) ? " class" : ""   \
, std::is_fundamental<decltype(z)>::value ? " funda" : ""   \
\
, std::is_bind_expression<decltype(z)>::value ? " bindExpr" : ""   \
, std::is_function<decltype(z)>::value ? " func" : ""   \
, std::is_null_pointer<decltype(z)>::value ? " nullptr" : ""   \
, std::is_void<decltype(z)>::value ? " void" : ""   \
, sizeof(decltype(z)), alignof(decltype(z)) \
)


#define PRaboutTYP(z) \
	printf("/Ln%d/  Type of expr \"\e[36m" #z "\e[0m\" has properties"  \
"\n\t"  \
" \e[32;3mtriv_copyable\e[0m?%c \e[32;3mtrivial\e[0m?%c"  \
" \e[32;3mhas_virtual_dtor\e[0m?%c"  \
" \e[32;3mdestructible\e[0m?%c{triv?%c;nothr?%c}"  \
"\n\t"  \
" \e[32;3mconstructible\e[0m?%c{triv?%c;nothr?%c}"  \
" \e[32;3mdflt_constructible\e[0m?%c{triv?%c;nothr?%c}"  \
"\n\t"  \
" \e[32;3mcopy_constructible\e[0m?%c{triv?%c;nothr?%c}"  \
" \e[32;3mmove_constructible\e[0m?%c{triv?%c;nothr?%c}"  \
"\n\t"  \
" \e[32;3mcopy_assignable\e[0m?%c{triv?%c;nothr?%c}"  \
" \e[32;3mmove_assignable\e[0m?%c{triv?%c;nothr?%c}"  \
"\n", __LINE__ \
\
, std::is_trivially_copyable<decltype(z)>::value ? 'Y' : 'N'   \
\
, std::is_trivial<decltype(z)>::value ? 'Y' : 'N'   \
\
, std::has_virtual_destructor<decltype(z)>::value ? 'Y' : 'N'   \
\
, std::is_destructible<decltype(z)>::value ? 'Y' : 'N'   \
, std::is_trivially_destructible<decltype(z)>::value ? 'Y' : 'N'   \
, std::is_nothrow_destructible<decltype(z)>::value ? 'Y' : 'N'   \
\
, std::is_constructible<decltype(z)>::value ? 'Y' : 'N'   \
, std::is_trivially_constructible<decltype(z)>::value ? 'Y' : 'N'   \
, std::is_nothrow_constructible<decltype(z)>::value ? 'Y' : 'N'   \
\
, std::is_default_constructible<decltype(z)>::value ? 'Y' : 'N'   \
, std::is_trivially_default_constructible<decltype(z)>::value ? 'Y' : 'N'   \
, std::is_nothrow_default_constructible<decltype(z)>::value ? 'Y' : 'N'   \
\
, std::is_copy_constructible<decltype(z)>::value ? 'Y' : 'N'   \
, std::is_trivially_copy_constructible<decltype(z)>::value ? 'Y' : 'N'   \
, std::is_nothrow_copy_constructible<decltype(z)>::value ? 'Y' : 'N'   \
\
, std::is_move_constructible<decltype(z)>::value ? 'Y' : 'N'   \
, std::is_trivially_move_constructible<decltype(z)>::value ? 'Y' : 'N'   \
, std::is_nothrow_move_constructible<decltype(z)>::value ? 'Y' : 'N'   \
\
, std::is_copy_assignable<decltype(z)>::value ? 'Y' : 'N'   \
, std::is_trivially_copy_assignable<decltype(z)>::value ? 'Y' : 'N'   \
, std::is_nothrow_copy_assignable<decltype(z)>::value ? 'Y' : 'N'   \
\
, std::is_move_assignable<decltype(z)>::value ? 'Y' : 'N'   \
, std::is_trivially_move_assignable<decltype(z)>::value ? 'Y' : 'N'   \
, std::is_nothrow_move_assignable<decltype(z)>::value ? 'Y' : 'N'   \
)


#include <iterator>

template<typename Iter>
using ItCateg = typename std::iterator_traits<Iter>::iterator_category;

#define PRbasicITERinfo(z) \
	printf("/Ln%d/  Iter \"\e[36m" #z "\e[0m\" is tagged..."  \
"\n\t\t\e[32m input_\e[0m,%c" \
"\n\t\t\e[32m output_\e[0m \e[2m(i.e., helper for adding ctainer elems)\e[0m,%c" \
"\n\t\t\e[32m forward_\e[0m,%c" \
"\n\t\t\e[32m bidirectional_\e[0m,%c" \
"\n\t\t\e[32m random_access_\e[0m,%c" \
"\n", __LINE__ \
, std::is_same<ItCateg<decltype(z)>,std::input_iterator_tag>() ? 'Y' : 'N'    \
, std::is_same<ItCateg<decltype(z)>,std::output_iterator_tag>() ? 'Y' : 'N'    \
, std::is_same<ItCateg<decltype(z)>,std::forward_iterator_tag>() ? 'Y' : 'N'    \
, std::is_same<ItCateg<decltype(z)>,std::bidirectional_iterator_tag>() ? 'Y' : 'N'   \
, std::is_same<ItCateg<decltype(z)>,std::random_access_iterator_tag>() ? 'Y' : 'N'   \
)

// TODO: figure how to get indirectly_readable, indirectly_writable, output_iterator
#if VER_ge20
#define PRextraITERinfo(z) \
	printf("/Ln%d/  Iter \"\e[36m" #z "\e[0m\" satisfs concepts..."  \
"\n\t\t\e[32m contiguous_\e[0m,%c" \
"\n\t\t\e[32m input_\e[0m,%c" \
"\n\t\t\e[32m incrementable\e[0m,%c" \
"\n\t\t\e[32m indirectly_swappable\e[0m,%c" \
"\n\t\t\e[32m permutable\e[0m,%c" \
"\n\t\t\e[32m sortable\e[0m,%c" \
"\n", __LINE__ \
, (std::contiguous_iterator<decltype(z)>) ? 'Y' : 'N'   \
, (std::input_iterator<decltype(z)>) ? 'Y' : 'N'   \
, (std::incrementable<decltype(z)>) ? 'Y' : 'N'   \
, (std::indirectly_swappable<decltype(z)>) ? 'Y' : 'N'   \
, (std::permutable<decltype(z)>) ? 'Y' : 'N'   \
, (std::sortable<decltype(z)>) ? 'Y' : 'N'   \
)
#else
#define PRextraITERinfo(z)
#endif



#ifdef __GXX_RTTI

#	include <typeinfo> // For typeid()
//
#	define nameTYPmangled(z) typeid(z).name()
//
#	define PRnameTYPmangled(z) \
	printf("/Ln%d/  Type of \"\e[36m" #z "\e[0m\" is \e[32m%s\e[0m\n", __LINE__, typeid(z).name())

#	include <cxxabi.h> // For abi::__cxa_demangle
//
int glo__demangleStatus;
char *glo__demangledName;
//
#	define PRtyp(z) \
	glo__demangledName = abi::__cxa_demangle((typeid(z).name()), NULL, NULL, &glo__demangleStatus);  \
	printf("/Ln%d/  Type of \"\e[36m" #z "\e[0m\" is \e[32m%s\e[0m; sizeof=%zu\n", __LINE__, (glo__demangleStatus ? "Demang Err!" : glo__demangledName), sizeof(decltype(z)));  \
	free(glo__demangledName);   \
	glo__demangledName = NULL

#endif //__GXX_RTTI


// Term "value category" is misleading; really, these are *expr* categories.
template<typename T>
constexpr const char *figureValueCategoryOfExpr (void) {
	// If VER_ge17, could say "if constexpr".
	if      (std::is_lvalue_reference<T>::value) { return "lvalue";  } // genuine Lvalue; resources mayn't be re-used.
	else if (std::is_rvalue_reference<T>::value) { return "xvalue";  } // eXpiring; resources can be re-used.
	else                                         { return "prvalue"; } // Pure Rvalue; computes some temporary.
}
//
#define PRexprValueCateg(z)   \
	printf("/Ln%d/  Expr \"\e[36m" #z "\e[0m\" is of value-category \e[1m%s\e[0m.\n", __LINE__, figureValueCategoryOfExpr<decltype(z)>())


/* Phase-4: figure out whether this non-bool integral is signed or not.
*/
template<typename T , typename U = typename std::make_signed<T>::type>
constexpr const char *reckonArithType__d ( ) {
	constexpr bool isSigned = std::is_same<T,U>::value;
#define checkTYPE(t) if (std::is_same<U,t>::value) { return isSigned ? #t : "unsigned " #t; }
	checkTYPE(char);
	checkTYPE(short);
	checkTYPE(int);
	checkTYPE(long);
	checkTYPE(long long);
#undef checkTYPE
	return "Some other integral type???";
}
/* Phase-3: switch between float-pt, bool, and non-bool integrals.  Must handle bool
   separately, because make_signed<bool> is an error.  Fun fact: although reckonArithType__c 
   has 3 definitions below, it is **not** overloaded --- for every arithmetic type, 2 of the 3
   definitions will be SFINAE'd out.  (And, the 3 signatures are identical.)
*/
template<typename T,
		 typename std::enable_if<
         		std::is_floating_point<T>::value >::type* = nullptr>
constexpr const char *reckonArithType__c () {
#define checkTYPE(t) if (std::is_same<T,t>::value) { return #t; }
	checkTYPE(float);
	checkTYPE(double);
	checkTYPE(long double);
#undef checkTYPE
	return "Some other floatpt type???";
}
template<typename T,
		 typename std::enable_if<
         		std::is_same<T,bool>::value >::type* = nullptr>
constexpr const char *reckonArithType__c () {
	return "bool";
}
template<typename T,
		 typename std::enable_if<
         		! std::is_floating_point<T>::value && ! std::is_same<T,bool>::value >::type* = nullptr>
constexpr const char *reckonArithType__c () {
		return reckonArithType__d<T>();
}
/* Phase-2: remove const-ness, if any.
*/
template<typename T , typename U = typename std::remove_const<T>::type>
constexpr const char *reckonArithType__b ( ) {
	return reckonArithType__c<U>();
}
/* Phase-1: remove reference-ness, if any.
*/
template<typename T , typename U = typename std::remove_reference<T>::type>
constexpr const char *reckonArithType__a() {
	return reckonArithType__b<U>();
}
//
#define PRarithType(z)   \
	printf("/Ln%d/  Expr \"\e[36m" #z "\e[0m\" is \e[1m%s\e[0m.\n", __LINE__, reckonArithType__a<decltype(z)>())



/*
#ifdef __EXCEPTIONS
#	warning "Exceptions enabled; re-compile with -fno-exceptions ?"
#endif
#ifdef __GXX_RTTI
#	warning "RTTI enabled; re-compile with -fno-rtti ?"
#endif
*/

////// C++-only section --- end //////
#endif //VER_ge11



#if VER_ge20
#	include <source_location>
#	define currFU (std::source_location::current().function_name())
#elif VER_ge11
	// GCC-specif; see https://gcc.gnu.org/onlinedocs/gcc/Function-Names.html
#	define currFU __PRETTY_FUNCTION__
#else
#	define currFU __func__
#endif

#define PRenteredFU \
	printf("\n/Ln%d/  Entered \e[33;7m%s\e[0m\n", __LINE__, __func__)

// // // "fu*" variants of some macros, as a shortcut.

#define fuPRlit(lit)  \
	PRlit(lit);       \
	printf("\t\t\\____ In fu \e[32m%s\e[0m\n", currFU)

#define fuPRmsg(printfFmt,...)      \
	PRmsg(printfFmt,__VA_ARGS__);   \
	printf("\t\t\\____ In fu \e[32m%s\e[0m\n", currFU)

#define fuPRtyp(z)  \
	PRtyp(z);       \
	printf("\t\t\\____ In fu \e[32m%s\e[0m\n", currFU)

#define fuPRval(z,printfFmt) \
	PRval(z,printfFmt);      \
	printf("\t\t\\____ In fu \e[32m%s\e[0m\n", currFU)

#define fuPRexprValueCateg(z)   \
	PRexprValueCateg(z);        \
	printf("\t\t\\____ In fu \e[32m%s\e[0m\n", currFU)


#endif //COMMON__H
