/*   (c)2024 Vainstein K.   */
#include "common.h"

float glob_f (char ca, char cb) { return 3.14F; }
float glob_g (char ca, char cb) { return 3.14F; }
//
void glob_h (long (&ref_to_arr_of_long)[3]) { }

struct Stru
{
	long _a[3];
	unsigned        _u;
	unsigned const  _cu;
	unsigned      * _pu;
	unsigned const* _pcu;
	Stru *_fp;
	Stru (const unsigned cu) : _u(9U), _cu(cu), _pu(&_u), _pcu(&_cu) {}
	Stru () =delete;
	float f (char ca, char cb) { return 3.14F; }
	float g (char ca, char cb) const { return 3.14F; }
	constexpr float h (char ca, char cb) { return 3.14F; }
};

// How ptr-to-memb func params look like
void globFFF (unsigned        Stru::* pmStru_u,
              unsigned  const Stru::* pmStru_cu,
              unsigned*       Stru::* pmStru_pu,
                              Stru& rs)
{	rs.*pmStru_u = 88U;   }

void globGGG (unsigned        Stru::* pmStru_u,
              unsigned  const Stru::* pmStru_cu) {}
void globHHH (unsigned        * pu,
              unsigned  const * pcu) {}


int main()
{
//============== refresher on ptrs, to make a point re ptr-to-const =======//
	bool        b{true};
	bool const cb{true};
	//
	bool const* pcb = &cb;
	            pcb = &b; //Always OK to point ptr-to-const, to a non-const...


//============== refresher on pointers to glob funcs ======================//
	float        (*p_fu)(char,char) =  glob_f;
	               p_fu             = &glob_f;

	float f00 = (*p_fu)('q','q');
	          // Dereferencing ptr-to-global-func is **optional**.
	float f01 =   p_fu ('q','q');


//============== ptr to a simple scalar (non-array) datamember ============//
	unsigned Stru::*       pm_u  = &Stru::_u;
///	                       pm_u  = &Stru::_cu; //ERR: not const!
	unsigned const Stru::* pm_cu = &Stru::_cu;


//============== ptr to a pointer-type datamember =========================//
	unsigned* Stru::*   pm_pu = &Stru::_pu;


//============== ptr to a member method ===================================//
	float (Stru::*pm_fu)(char,char)        =  Stru::f;
	              pm_fu                    = &Stru::f;
///	              pm_fu                    = &Stru::g; //ERR: not const!
///	              pm_fu                    = &Stru::h; //ERR: not const!
	                           // even though h() is constexpr, not const.

	float (Stru::*pm_cfu)(char,char) const = &Stru::g;
///	              pm_cfu                   = &Stru::f; //ERR: wrong type!
	                     //...Not *that* kind of const, eh?
#if VER_eq11
	              pm_cfu                   = &Stru::h;
            // Conclude: for ptr-to-member-fu, constexpr as good as const. ...until 14??
#endif

//============== binding ptr-to-memb, to member ============================//
	Stru        s{33U};
	Stru const cs{55U};

	Stru      * ps  = &s;
	Stru const* pcs = &cs;
	Stru      & rs  =  s;
	Stru const& rcs =  cs;

		/*ERR:  ".*" and "->*"  are operators; can't intersperse
		spaces into them, anymore than into "->"!  Eg "pfoo-  >bar" */
///	s.  *pm_u = 77U;
	s.*  pm_u = 77U;    // Can do exactly same through ptr or ref:
	ps->*pm_u = 77U;
	rs.* pm_u = 77U;

///	float f03 = ps->*pm_fu('z','z');
	          // Dereferencing ptr-to-member-func is **required**.
	float f04 = (s.*  pm_fu)('z','z');
	float f05 = (ps->*pm_fu)('z','z');
	float f06 = (rs.* pm_fu)('z','z'); // Exactly equiv to how we got f04.

///	float f07 = (pcs->*pm_fu)('z','z');  // ERR: const/nonconst mismatch
	float f08 = (pcs->*pm_cfu)('z','z');
	float f09 = (rs  .*pm_cfu)('z','z');


//============== ptr to an array datamember ================================//
	long a[3];
	const long ca[3] = {111L,222L,333L};
	glob_h(a);
///	glob_h(ca);           //ERR: discards 'const' qual

	long (Stru::*pm_a)[3] = &Stru::_a;
	glob_h(ps->*pm_a);
///	glob_h(pcs->*pm_a);   //ERR: discards 'const' qual


//============== passing ptr-to-memb args to func ==========================//
	globFFF(pm_u, pm_cu, pm_pu, s);

	globGGG(pm_u,  pm_cu);
///	globGGG(pm_cu, pm_u);
///	globGGG(pm_cu, pm_cu);
	globGGG(pm_u,  pm_u);

	unsigned        u_on_stack,      * puStack = &u_on_stack;
	unsigned const cu_on_stack{66U}, *pcuStack = &cu_on_stack;
	globHHH( puStack, pcuStack);
///	globHHH(pcuStack,  puStack);
///	globHHH(pcuStack, pcuStack);
	globHHH( puStack,  puStack);


//============== yes, ptr-to-memb is a pointer (sort of) ===================//
///	pm_u = &u_on_stack;  //ERR: incompatible types
	pm_u = nullptr;
}
