/*   (c)2024 Vainstein K.   */
/*
Fold expressions are relatively simple: they always involve binary ops, and
always act on parampacks.  No execution policies to choose from.
*/
#include "common.h"



/* Fold expressions: apply binary operator to all elems of a parampack.
*/
#if VER_ge17
template<typename... Ts> // <--- te params
decltype(auto) fold_subA (Ts... args) { // <--- fu params
	fuPRmsg("Size of te parampack: %zu\n", sizeof...(Ts));
	fuPRmsg("Size of fu parampack: %zu\n", sizeof...(args));
	return (... - args);                 // unary left fold   ---  "left-associative"
}
template<typename... Ts>
decltype(auto) fold_subB (Ts... args) {
	return (args - ...);                 // unary right fold  ---  "right-associative"
}
template<typename... Ts>
decltype(auto) fold_subC (Ts... args) {
	fuPRmsg("Size of te parampack: %zu\n", sizeof...(Ts));
	return (100 - ... - args);           // binary left fold  ---   "left-associative"
}
template<typename... Ts>
decltype(auto) fold_subD (Ts... args) {
	fuPRmsg("Size of fu parampack: %zu\n", sizeof...(args));
	return (args - ... - 100);           // binary right fold  ---  "right-associative"
}

template<typename... Ts>
decltype(auto) fold_lshiftB (Ts... args) {
	return (args << ...);                // unary right fold
}
template<typename... Ts>
decltype(auto) fold_lshiftD (Ts... args) {
	return (args << ... << 3LU);         // binary right fold
}

template<typename... Ts>
decltype(auto) fold_addC (Ts... args) {
	return (0 + ... + args);
}
#endif


void test__foldExprs () {
	PRenteredFU;
#if VER_ge17
	SAYevalPRret(  fold_subA(20,7,9)  ,"%d");   //   (20-7)-9
	SAYevalPRret(  fold_subB(20,7,9)  ,"%d");   //    20-(7-9)
	SAYevalPRret(  fold_subC(20,7,9)  ,"%d");   //   ((100-20)-7)-9
	SAYevalPRret(  fold_subD(20,7,9)  ,"%d");   //   20-(7-(9-100))
	//
	SAYevalPRret(  fold_lshiftB(7LLU,5LLU,3LLU)   ,"%llu");   //     7<<(5<<3)
	SAYevalPRret(  fold_lshiftD(7LLU,5LLU)        ,"%llu");   //     7<<(5<<3)
	static_assert(std::is_same_v<decltype(fold_lshiftB(7LLU,3LLU)), long long unsigned>);
	//
	// Big advantage of the binary folds is won't barf if called with *no* args:
	SAYevalCHKret(  fold_addC()  ,"%d", 0);
	// If binary fold with initial-val clause called with no args, get back the initial-val.
	SAYevalCHKret(  fold_subC()  ,"%d", 100);
	SAYevalCHKret(  fold_subD()  ,"%d", 100);/* Ret type depends on how initial-val literal
	                                             was written; if "100L", would return long. */
	//
	// If a unary fold called with just *one* arg, ought to simply return it:
	SAYevalCHKret(  fold_subA(42)  ,"%d", 42);
	SAYevalCHKret(  fold_subB(42)  ,"%d", 42);
#endif
}


int main () {
    test__foldExprs();
}
