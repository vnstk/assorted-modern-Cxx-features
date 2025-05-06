/*   (c)2025 Vainstein K.   */
/*
	"value category" is a misnomer; they really are **expression** categories.
*/
#include "common.h"


#undef  fuPRmsg
// Re-defining, because want extra newline at the end.
#define fuPRmsg(printfFmt,...)      \
	PRmsg(printfFmt,__VA_ARGS__);   \
	printf("\t\t\\____ In fu \e[32m%s\e[0m\n\n", currFU)


struct Qux {
	float _flo;
	Qux (float flo) : _flo(flo) {   fuPRmsg("_flo=%.2f\n",_flo);   }
	Qux (Qux const& rhs) : _flo(rhs._flo) {   fuPRmsg("_flo=%.2f\n",_flo);   }
	Qux (Qux&& rhs) : _flo(std::move(rhs._flo)) {   fuPRmsg("_flo=%.2f\n",_flo);   }
};

#if 0
Qux&  defined_ret_lvalue() {
	static Qux qStatic{55.5};
	return qStatic;
}
Qux&& defined_ret_xvalue(Qux& qParam) {
	return std::move(qParam);
}
Qux   defined_ret_prvalue() {
	Qux qStack{66.6};
	return qStack;
}

void test__query_retval_of_defined_fu()
{	PRenteredFU;
	auto qa = defined_ret_lvalue();
	PRwhat(qa);
	PRexprValueCateg(qa);

	Qux qArg{77.7};
	auto qb = defined_ret_xvalue(qArg);
	PRwhat(qb);
	PRexprValueCateg(qb);

	auto qc = defined_ret_prvalue();
	PRwhat(qc);
	PRexprValueCateg(qc);
}
#endif

int&  ret_lvalue(); // Needn't define.
int&& ret_xvalue();
int   ret_prvalue();
/* Should be possible to just ask for ret type of ret_lvalue(),
  but can't see right now how to.  OK then, workaround:
*/
struct DummyFuncObj {
	int&  operator()(char);
	int&& operator()(char,char);
	int   operator()(char,char,char);
};


void test__query_rettype_of_undefined_fu()
{	PRenteredFU;

	PRmsg("expr categ: %s\n", figureValueCategoryOfExpr<
			std::result_of<DummyFuncObj(char)>::type
		>());
	PRmsg("expr categ: %s\n", figureValueCategoryOfExpr<
			std::result_of<DummyFuncObj(char,char)>::type
		>());
	PRmsg("expr categ: %s\n", figureValueCategoryOfExpr<
			std::result_of<DummyFuncObj(char,char,char)>::type
		>());

	SAYevalCHKretCSTR(  figureValueCategoryOfExpr<std::result_of<DummyFuncObj(
		char
		)>::type>() , "lvalue");
}



void test__query_rettype_of_inaccessible_fu()
{	PRenteredFU;

}



template<typename T>
char const * f0 (T x) {
	fuPRmsg("expr categ: %s\n", figureValueCategoryOfExpr<decltype(x)>());
	return figureValueCategoryOfExpr<decltype(x)>();
}
char const * f1 (int const x) {
	fuPRmsg("expr categ: %s\n", figureValueCategoryOfExpr<decltype(x)>());
	return figureValueCategoryOfExpr<decltype(x)>();
}
char const * f2 (int& x) {
	fuPRmsg("expr categ: %s\n", figureValueCategoryOfExpr<decltype(x)>());
	return figureValueCategoryOfExpr<decltype(x)>();
}
char const * f3 (int const& x) {
	fuPRmsg("expr categ: %s\n", figureValueCategoryOfExpr<decltype(x)>());
	return figureValueCategoryOfExpr<decltype(x)>();
}
char const * f4 (int&& x) {
	fuPRmsg("expr categ: %s\n", figureValueCategoryOfExpr<decltype(x)>());
	return figureValueCategoryOfExpr<decltype(x)>();
}
char const * f5 (int const&& x) {
	fuPRmsg("expr categ: %s\n", figureValueCategoryOfExpr<decltype(x)>());
	return figureValueCategoryOfExpr<decltype(x)>();
}
template<typename T>
char const * f6 (T&& x) {
	fuPRmsg("expr categ: %s\n", figureValueCategoryOfExpr<decltype(x)>());
	return figureValueCategoryOfExpr<decltype(x)>();
}

void test__valueCategories()
{	PRenteredFU;
	fuPRmsg("expr categ: %s\n", figureValueCategoryOfExpr<decltype(ret_lvalue)>());


		int iA{42};
		Qux qA{11.1F};
		const int iB{42};
		constexpr int iC{42};
		int& riA = iA;
		const int& riB = iB;

		SAYevalCHKret(  f0(42)   , "%s", "prvalue" );
		SAYevalCHKret(  f1(42)   , "%s", "prvalue" );
		SAYevalCHKret(  f4(42)   , "%s",  "xvalue" );  //XXX --- why not prvalue?
		SAYevalCHKret(  f5(42)   , "%s",  "xvalue" );

		SAYevalCHKret(  f0(iA)   , "%s", "prvalue" );
		SAYevalCHKret(  f1(iA)   , "%s", "prvalue" );
//		SAYevalCHKret(  f4(iA)   , "%s",  "xvalue" );  //XXX --- why not prvalue?
//		SAYevalCHKret(  f5(iA)   , "%s",  "xvalue" );

		SAYevalCHKret(  f0(qA)   , "%s", "prvalue" );
//		SAYevalCHKret(  f1(qA)   , "%s", "prvalue" );
//		SAYevalCHKret(  f4(qA)   , "%s",  "xvalue" );  //XXX --- why not prvalue?
//		SAYevalCHKret(  f5(qA)   , "%s",  "xvalue" );

#if 0
		SAYeval(  f0(iA)  );
		SAYeval(  f1(iA)  );
		SAYeval(  f0(iB)  );
		SAYeval(  f1(iB)  );
		SAYeval(  f2(riA)  );
		SAYeval(  f3(riB)  );
		SAYeval(  f4(42)  );
		SAYeval(  f4(int{42})  );
		SAYeval(  f4(std::move(int{42}))  );
		int iD{42};
		SAYeval(  f4(std::move(iD))  );
	//	SAYeval(  f4(iC)  );   //Err,  cannot bind rvalue reference of type 'int&&' to lvalue of type 'const int'
		SAYeval(  f6(iA)  );
		SAYeval(  f6(iB)  );
		SAYeval(  f6(riA)  );
		SAYeval(  f6(std::move(riA))  );
		SAYeval(  f6(riB)  );
		SAYeval(  f6(std::move(iA))  );
		SAYeval(  f6(42)  );
		SAYeval(  f6(std::move(riB))  );
#endif
}


int main() {
	test__query_retval_of_defined_fu();
	test__query_rettype_of_undefined_fu();
	test__query_rettype_of_inaccessible_fu();
return 0;
	test__valueCategories();
}
