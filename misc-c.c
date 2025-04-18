/*   (c)2024 Vainstein K.   */
#include "common.h"


void test__static_assert() {
#if cVER_ge11
	_Static_assert(sizeof(int) >= 4);
#endif
#if cVER_ge23
	static_assert(sizeof(int) >= 4);
#endif
}

void test__typeof() {
#if cVER_ge23
	float fa = 3.14F;
	const float fb = 3.14F;
	typeof(fa) *pfa = &fa;
	typeof(fb) *pfb = &fb;

	typeof_unqual(fa) *upfa = &fa; //fine...
///	typeof_unqual(fb) *upfb = &fb; //ERRor!!  (Well, WARNing.)  Because _unqual strips const-qualifier.
#endif
}


// Calls must provide ptr to 0th elem of arry with at least 3 elems!
void f_arrayConstraintX (float fa[static 3]) {}

// Calls must provide ptr to a **const** float arry.
void f_arrayConstraintY (const float cfa[]) {}

void test__array_cstraints() {

	float a0[0] = {};
	float a1[1] = {1.1F};
	float a2[2] = {1.1F, 2.2F};
	float a3[3] = {1.1F, 2.2F, 3.3F};
	float a4[4] = {1.1F, 2.2F, 3.3F, 4.4F};
	float a5[5] = {1.1F, 2.2F, 3.3F, 4.4F, 5.5F};
	//
	f_arrayConstraintX(a4); //fine...
	f_arrayConstraintX(a3); //fine...
	f_arrayConstraintX(a2); //    ...ERRor!  (Well, WARNing.)  Because not GE 3 elems in passed arry.

	const float ca0[0] = {};
	//
	f_arrayConstraintY(ca0); //fine...
	f_arrayConstraintY(a0); //    ...ERRor!  (Well, WARNing.)   Because passed arry not const.
}



int main ()
{
	test__static_assert();
	test__typeof();
	test__array_cstraints();

/*
	test__VLA(); // variable-length arry --- TODO TODO
*/




}

