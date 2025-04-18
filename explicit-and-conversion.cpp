/*   (c)2024 Vainstein K.   */
#include "common.h"
#include <string>


struct A {
    A() { fuPRlit("cvertg dflt ctor"); }
    A(float) { fuPRlit("cvertg nondflt ctor"); }
	//
    A(A const&) { fuPRlit("cvertg copy ctor"); }
    A(A&&) { fuPRlit("cvertg move ctor"); }
	//
	template<typename T>
    operator T() const {
		fuPRlit("cvertg conv-to-T");
		T dfltCstructedTemp;
		return dfltCstructedTemp + 3;
	}
};

struct B {
    explicit B() { fuPRlit("expli dflt ctor"); }
    explicit B(float) { fuPRlit("expli nondflt ctor"); }
	//
	template<typename Tnumeric, Tnumeric val>
    explicit B(const char *) { fuPRlit("expli nondflt ctor, fu te"); }
    explicit B(B const&) { fuPRlit("expli copy ctor"); }
    explicit B(B&&) { fuPRlit("expli move ctor"); }
	//
	template<typename T>
    explicit operator T() const {
		fuPRlit("expli conv-to-T");
		T dfltCstructedTemp;
		return dfltCstructedTemp + 3;
	}
};

#if VER_ge20
struct C {
	template<typename Tnumeric>
	explicit (sizeof(Tnumeric) >= 4)
    C(const char*, Tnumeric) { fuPRlit("cvertg nondflt ctor, fu te"); }
	//
    explicit C() { fuPRlit("expli dflt ctor"); }
	C(float f) { fuPRlit("expli nondflt ctor"); }
    explicit C(C const&) { fuPRlit("expli copy ctor"); }
    explicit C(C&&) { fuPRlit("expli move ctor"); }
	//
	template<typename T>
	explicit (sizeof(T) >= 4)
	operator T() const {
		fuPRlit("expli conv-to-T");
		T dfltCstructedTemp;
		return dfltCstructedTemp + 3;
	}
};
#endif

int main()
{
	{
		SAYeval(A x00{3.3F}   );
		SAYeval(A x01{x00}    );
		SAYeval(A x02 = x00  );
		SAYeval(A x03{std::move(x01)}  );
		SAYeval(A x04   );
		SAYeval(A x05 = 3.3F   );
		SAYeval(A x06 = (A)3.3F   );
		SAYeval(uint16_t u00 = x06   );                        // implicit cversion.
		SAYeval(uint16_t u01 = static_cast<uint16_t>(x03)   ); // explicit cversion.
	}

	{
		SAYeval(B x00{3.3F}   );
		SAYeval(B x01{x00}    );
//		SAYeval(B x02 = x00  );  //ERR, because copy ctor is explicit.
		SAYeval(B x03{std::move(x01)}  );
		SAYeval(B x04   );
//		SAYeval(B x05 = 3.3F   ); //ERR, because nondflt ctor is explicit.
		SAYeval(B x06 = (B)3.3F   );
//		SAYeval(uint16_t u00 = x06   );  //ERR, because conv-to-T is explicit.
		SAYeval(uint16_t u01 = static_cast<uint16_t>(x03)   );
	}

	{
		SAYeval(C x00{3.3F}   );
		SAYeval(C x01{x00}    );
		SAYeval(C x03{std::move(x01)}  );

#ifdef __cpp_conditional_explicit
		C x08{"huh", (uint8_t)42}; //????? shouldn't cpile
		C x09{"huh", 42L};
		SAYeval(uint16_t u01 = static_cast<uint16_t>(x03)   );
		SAYeval(uint8_t  u02 = static_cast<uint8_t >(x03)   ); //????? shouldn't cpile
#endif
	}

}

/*
	o	"explicit" keywd, may adorn:
		o	ctors
			o	nondflt, plain
			o	nondflt, fu te
			o	copy
			o	move
		o	cversion fu
			o	plain
			o	fu te
	o	As above, but with a bool-valued cpiletime-constant expr in parens afterw. //20
	o	Examp of implicit cversion and copy-inizn (situats where explicit ctor or cversion fu mayn'y be used)


https://en.cppreference.com/w/cpp/language/constant_expression#Converted_constant_expression
https://en.cppreference.com/w/cpp/language/converting_constructor


Explicit constructors, which are only considered during direct initialization (which includes explicit conversions such as static_cast), converting constructors are also considered during copy initialization, as part of user-defined conversion sequence.
*/
