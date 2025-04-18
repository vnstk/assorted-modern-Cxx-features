/*   (c)2024 Vainstein K.   */
#include "common.h"

#include <setjmp.h>
jmp_buf globJmpEnv;

static double globFp = -1.111;

void fd (const char *args) {
	fuPRlit("Entered fu");
}

void fc (double argd) {
	fuPRmsg("Entered fu, argd=%.3f\n", argd);
	if (argd > 7.0) {
		globFp = 7.77;
		SAYeval(  longjmp(globJmpEnv,77)  );
	} else if (argd > 5.0) {
		globFp = 5.55;
		SAYeval(  longjmp(globJmpEnv,55)  );
	}
	fuPRlit("Leaving fu");
}


void fb (float argf) {
	fuPRlit("Entered fu");
	fc(argf);
	fuPRlit("Leaving fu");
}

static void fa (unsigned argu) {
	fuPRmsg("Entered fu, argu=%u\n", argu);
	globFp = 1.11;
	volatile char locCha = 'z';
	char locButNotVolat = 'z';

	const int ret__setjmp = setjmp(globJmpEnv);
	fuPRmsg("argu=%u ret__setjmp=%d globFp=%.2f locCha=%c locButNotVolat=%c\n",
	        argu, ret__setjmp, globFp, locCha, locButNotVolat);
	locCha = 'q';   locButNotVolat = 'q';
/*
	Check that statics are as were when longjmp() was called.
	Check that locals are correct only if volatile --- otherw
		not guaranteed.
*/
	if (ret__setjmp == 0) { // First time through?
		globFp = 2.22;
		locCha = 'r';   locButNotVolat = 'r';
		fb((float)argu + 0.123F);
		locCha = 'R';   locButNotVolat = 'R';
	} else {
		globFp = 3.33;
		locCha = 's';   locButNotVolat = 's';
		fd("flarp");
	}
	locCha = 't';   locButNotVolat = 't';
	fuPRlit("Leaving fu");
}


int main () {
//	PRmsg("__STDC_VERSION__ = %ld\n", __STDC_VERSION__);
	fuPRlit("Entered fu");

	SAYeval(  fa(6U)  );
	globFp = 4.44;
	SAYeval(  fa(8U)  );

	fuPRlit("Leaving fu");
}
