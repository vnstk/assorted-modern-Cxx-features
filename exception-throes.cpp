/*   (c)2025 Vainstein K.   */
//XXX Must compile this with   -fexceptions
#include "common.h"
#include <cstdlib> //For  exit()
#include <exception>

class FooException : public std::exception {
};
class BarException : public std::exception {
};

struct Qux {
	void f_it_throws_for_sure() {
		throw new BarException();
	}
	void f_doesnSayItThrows_andDoesntThrow () {
#if NULL      //Could've equivalently said
	void f_doesnSayItThrows_andDoesntThrow () noexcept(false) {
#endif
	}
	void f_saysItDoesntThrow_andDoesntThrow () noexcept {
	}
	void f_doesnSayItThrows_butThrows () {
		try {
			f_it_throws_for_sure();
		} catch (BarException const&) {
			throw new FooException();
		}
	}
#if 0
	void f_saysItDoesntThrow_butThrows () noexcept {
		try {
			f_it_throws_for_sure();
		} catch (BarException const&) {
			throw new FooException();
		}
	}
#endif
	~Qux() {
#if VER_ge17
		if (int nThrowUps = std::uncaught_exceptions()) {
			fuPRmsg("Exactly %d exceptions thrown.\n", nThrowUps);
//XXX can't get this message to print.  Yes tried flushing.
#else
		if (std::uncaught_exception()) {
			fuPRlit("In process of dying violently.");
#endif
		} else {
			fuPRlit("peacefully dying of old age");
		}
	}
};



void test__uncaught_indicators ()
{	PRenteredFU;

	std::terminate_handler set_terminate( [](){
		fflush(stdout);
		std::abort();//exit(3);
	} );

	SAYeval(   Qux{}   );
	SAYeval(  (Qux{}).f_doesnSayItThrows_andDoesntThrow()   );
	SAYeval(  (Qux{}).f_doesnSayItThrows_andDoesntThrow()   );
	SAYeval(  (Qux{}).f_doesnSayItThrows_butThrows()        );

}


int main () {
	test__uncaught_indicators();
}


#if 0

move_if_noexcept


https://en.cppreference.com/w/cpp/language/noexcept_spec

https://en.cppreference.com/w/cpp/language/noexcept

https://en.cppreference.com/w/cpp/keyword/noexcept
#endif
