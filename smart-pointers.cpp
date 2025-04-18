/*   (c)2024 Vainstein K.   */
#include "common.h"
#include <memory> // For unique_ptr

//XXX: haven't been able to detect any difference in behv betw make_unique and make_unique_for_overwrite

struct Foo {
	float _f{3.14f};
	unsigned _u{111U};
	Foo ( ) {
		printf(   "Foo  dflt ctor, _u=%u; this=%p\n", _u, this); }
	Foo (unsigned u) : _u(u) {
		printf("Foo non-dflt ctor, _u=%u; this=%p\n", _u, this); }
	~Foo () {
		printf(         "Foo dtor, _u=%u; this=%p\n", _u, this); }
};

struct Bar {
	double _dummy[5]; // So that &myBar._foo won't be same as &myBar
	Foo _foo{222U};
	const char *_cs;
	Bar ( ) {
		printf(    "Bar dflt ctor, _foo._u=%u; this=%p &_foo=%p\n", _foo._u, this, &_foo);
	}
	Bar (unsigned u, const char *cs) : _foo(u), _cs(cs) {
		printf("Bar non-dflt ctor, _foo._u=%u; this=%p &_foo=%p ;_cs=\"%s\"\n",
		             _foo._u, this, &_foo, _cs); }
	~Bar () {
		printf(         "Bar dtor, _foo._u=%u; this=%p &_foo=%p\n",
		             _foo._u, this, &_foo); }
};


void test__unique_ptr__noFrills ()
{	PRenteredFU;

	{
		std::unique_ptr<Foo> u0 = std::unique_ptr<Foo>(new Foo(10U));
		std::unique_ptr<Foo> u1(new Foo(11U));
		std::unique_ptr<Foo> u2(new Foo(12U));

		// unidirectionally transfer ownership with move
		std::unique_ptr<Foo> u3;
		assert(  u0);
		assert(! u3);
		assert(nullptr == u3.get());
		u3 = std::move(u0);
		assert(! u0);
		assert(  u3);

		// swap (bidirectional) the owned objs
		assert((11U == u1->_u) && (12U == u2->_u));
		std::swap(u2, u1);
		assert(u1);
		assert(u2);
		assert((11U == u2->_u) && (12U == u1->_u));

		// unidirectional transfer will trash whatever dest had owned previously.
		assert(  u2);
		assert(  u3);
		PRlit("About to       u2 = std::move(u3);");
		u2 = std::move(u3);
		assert(  u2);
		assert(! u3);
		// An exception to the "a moved-out-of obj will have indeterminate but valid state" 
		// rule: moved-out-of unique_ptr will be empty -- guaranteed.  Same with shared_ptr.
	}

	{	// owns array?
		PRlit("77,88,99");
		std::unique_ptr<Foo[]> ua( new Foo[]{77U,88U,99U} );
		SAYevalCHKret( ua[2]._u , "%u" , 99U );
		assert(99U == ua[2]._u);
	}	//NB, dtors run in reverse order: Foo(99U), then Foo(88U), then Foo(77U).

	{	
#if VER_ge14
		std::unique_ptr<Foo> b1 = std::make_unique<Foo>(12U);

		PRlit("Shall                   make_unique<Bar>(15U)");
		std::unique_ptr<Bar> b2 = std::make_unique<Bar>(15U, "Make_unique");

		PRlit("Shall  make_unique<Bar[]>(5)");
		auto b3{ std::make_unique<Bar[]>(5) };
#endif
	}	// make_unique and make_shared do value-inizn (ini val discarded);

	{	// make_unique_for_overwrite and make_shared_for_overwrite do dflt-ininz.
#if VER_ge20
		PRlit("Shall  make_unique_for_overwrite<Bar[]>(3)");
		auto b4{ std::make_unique_for_overwrite<Bar[]>(3) };
	// XXX How is this any different from make_unique  ????????
#endif
	}
}



struct Stru_deleterE;

struct Zoo {
	unsigned _u{111U};
	Zoo ( ) {
		printf(   "Zoo  dflt ctor, _u=%u; this=%p\n", _u, this); }
	Zoo (unsigned u) : _u(u) {
		printf("Zoo non-dflt ctor, _u=%u; this=%p\n", _u, this); }
private:
	~Zoo () { printf("top-secret Zoo dtor, _u=%u; this=%p\n", _u, this); }
public:
	static void deleterA (Zoo *pzoo) {
		fuPRmsg("about to trash Zoo, _u=%u; its this=%p\n", pzoo->_u, pzoo);
		delete pzoo; }
	static void deleterB (const Zoo *pzoo) {
		fuPRmsg("about to trash Zoo, _u=%u; its this=%p\n", pzoo->_u, pzoo);
		delete pzoo; }
	static unsigned deleterC (Zoo *pzoo) {
		fuPRmsg("about to trash Zoo, _u=%u; its this=%p\n", pzoo->_u, pzoo);
		const unsigned copy_u = pzoo->_u;
		delete pzoo;
		return copy_u; }
	static void deleterD (Zoo pzoo[]) {
		fuPRmsg("about to trash Zoo[], 0th elt whereof is %p\n", &pzoo[0]);
		delete[]pzoo;
	}
	friend struct Stru_deleterE;
};

struct Stru_deleterE {
	void operator() (Zoo *pzoo) {
		fuPRmsg("about to trash Zoo, _u=%u; its this=%p\n", pzoo->_u, pzoo);
		delete pzoo;
	}
};

using Zoodeleter_t = std::function<void(Zoo*)>;
using arrZoodeleter_t = std::function<void(Zoo[])>;

void test__unique_ptr__customDeleter ()
{	PRenteredFU;
#if 0
	std::unique_ptr<Zoo> uz0; // Illegal, because dtor private.
#endif
	std::unique_ptr<Zoo,Zoodeleter_t> uz33( new Zoo(33U) , Zoo::deleterA );
	// Taking "const Zoo*" and not "Zoo*" ?  No problem!
	std::unique_ptr<Zoo,Zoodeleter_t> uz44( new Zoo(44U) , Zoo::deleterB );
	// Returning "unsigned" and not "void" ?  No problem!
	std::unique_ptr<Zoo,Zoodeleter_t> uz55( new Zoo(55U) , Zoo::deleterC );
	Stru_deleterE deleterE;
	std::unique_ptr<Zoo,Zoodeleter_t> uz66( new Zoo(66U) , deleterE );
	std::unique_ptr<Zoo,Zoodeleter_t> uz66alt( new Zoo(6666U) , Stru_deleterE() );
	//
//	std::unique_ptr<Zoo[],arrZoodeleter_t> uz789( new Zoo[3] , Zoo::deleterA ); // compiles but crashes!
	std::unique_ptr<Zoo[],arrZoodeleter_t> uz789( new Zoo[3] , Zoo::deleterD );
}



void test__shared_ptr__aliasingCtor ()
{	PRenteredFU;
	/*                                       "Aliasing" ctor,
			template< class Y >
			shared_ptr( const shared_ptr<Y>& r, element_type* ptr ) noexcept;
	*/
/* If this shared_ptr is the last of the group to go out of scope, it will call
   the stored deleter for the object originally managed by r. */
/* However, calling get() on this shared_ptr will always return a copy of ptr. */
/* Programmer must make sure that said ptr remains valid as long as this shared_ptr
   exists, such as in the typical use cases where ptr is a member of the object
   managed by r or is an alias (e.g., downcast) of r.get() */

	{
		std::shared_ptr<Bar> shab = std::make_shared<Bar>(17U, "Make_shared");
		PRmsg("Ini, shab.use_count()=%zu ; shab.get()=%p\n", shab.use_count(), shab.get());
		//
		std::shared_ptr<Foo> shaf(/*shares ownership info with:*/ shab, 
		                          /*holds unrelated unmanaged ptr:*/ &shab->_foo);
		PRmsg("Now, shab.use_count()=%zu ; shaf.get()=%p\n", shab.use_count(), shaf.get());
		//
		// Expect false, because they're sharing ownership info --- right??
		SAYevalCHKretBOOL(   shab.owner_before(shaf)   ,false);
	}

#if VER_ge20
	printf("===============\e[1;3m Repeat, but using the takes-rval oload"
           " of shared_ptr ctor. \e[0m===============\n");
	/*		template< class Y >
			shared_ptr( shared_ptr<Y>&& r, element_type* ptr ) noexcept;//20
	*/
	{
		std::shared_ptr<Bar> shab = std::make_shared<Bar>(17U, "make_shared");
		PRmsg("Ini, shab.use_count()=%zu ; shab.get()=%p\n", shab.use_count(), shab.get());
		//
		Foo *p_shab_foo = &shab->_foo;
		std::shared_ptr<Foo> shaf(/*shares ownership info with:*/ std::move(shab), 
		                          /*holds unrelated unmanaged ptr:*/ p_shab_foo);
		PRmsg("Now, shaf.use_count()=%zu ; shaf.get()=%p\n", shaf.use_count(), shaf.get());
	}
#endif
}


struct Qux : std::enable_shared_from_this<Qux> {
	float _f{3.14f};
	Qux ( ) {
		printf(   "Qux  dflt ctor, _f=%.1f; this=%p\n", _f, this); }
	Qux (float f) : _f(f) {
		printf("Qux non-dflt ctor, _f=%.1f; this=%p\n", _f, this); }
	~Qux () {
		printf(         "Qux dtor, _f=%.1f; this=%p\n", _f, this); }
};

void test__enable_shared_from_this()
{	PRenteredFU;

	Qux                 *rawp_q0 = new Qux(              44.4F);
	Qux                       q1{                        55.5F};
	std::unique_ptr<Qux> uptr_q2( new Qux(66.6F) );

	std::shared_ptr<Qux> shaA(rawp_q0);
	PRmsg("shaA.use_count=%zu shaA.get()=%p\n", shaA.use_count(), shaA.get());

	std::shared_ptr<Qux> shaB = rawp_q0->shared_from_this();
	PRmsg("shaA.use_count=%zu shaB.get()=%p\n", shaB.use_count(), shaA.get());

#if NULL // Do not do either of these!
	std::shared_ptr<Qux> shaBadIdeaOne = q1.shared_from_this(); //==> a smashed stack
	std::shared_ptr<Qux> shaBadIdeaTwo = uptr_q2->shared_from_this(); //==> crash with "double free or corruption" runtime error 
#endif
}


void test__shared_ptr__miscOther ()
{	PRenteredFU;
	{	// basics
		std::unique_ptr<Foo> u(new Foo(42U));
		assert(u);

//		std::shared_ptr<Foo> sh( u ); // Illegal!
		std::shared_ptr<Foo> sh( std::move(u) );
		assert(sh);
		assert(!u);

		std::shared_ptr<Foo> sh2( sh );
		assert(2 == sh2.use_count());

		std::shared_ptr<Foo> sh3( sh );
		assert(3 == sh3.use_count());

		std::shared_ptr<Foo> sh4( std::move(sh) );
		assert(3 == sh4.use_count());
	}
	{	// Array
		PRlit("77,88,99");
		std::shared_ptr<Foo[]> sha( new Foo[]{77U,88U,99U} );
		SAYevalCHKret( sha[2]._u , "%u" , 99U ); // Should require at least 17, but doesn't.  Weird.
	}
	{	// Custom deleter
		std::shared_ptr<Zoo> shz22( new Zoo(22U) , Zoo::deleterA );
		std::shared_ptr<Zoo> shz33( new Zoo(33U) , Stru_deleterE() );
	}	// XXX How come Deleter type not a te param, as was the case with unique_ptr??

	{	// owner_before ???
		std::shared_ptr<Bar> shA(new Bar(42U, "xxx"));
		std::shared_ptr<Bar> shB(shA);
		std::shared_ptr<Bar> shC(shB);
//		std::weak_ptr<Foo> wX(
		std::shared_ptr<Bar> shD(shA);
		std::shared_ptr<Foo> shE(&shA->_foo);
		std::shared_ptr<Bar> shF(new Bar(123U, "yyy"));
		PRmsg("shA %s shB\n", shA.owner_before(shB) ? "owner_before" : "???");
		PRmsg("shA %s shF\n", shA.owner_before(shF) ? "owner_before" : "???");
		PRmsg("shB %s shC\n", shB.owner_before(shC) ? "owner_before" : "???");
		//TODO: figure out.
	}
	// owner_before is owner-based; so is owner_less.  What's this mean???
}


void test__weak_ptr ()
{	PRenteredFU;
	{
		std::unique_ptr<Foo> u(new Foo(333U));
		std::shared_ptr<Foo> sh( std::move(u) );
		assert(1U == sh.use_count());
		std::weak_ptr<Foo> wa( sh );
		assert(1U == sh.use_count());
		std::weak_ptr<Foo> wb( wa );
		assert(1U == sh.use_count());

	}


/*
bad_weak_ptr
exception thrown when accessing a weak_ptr which refers to already destroyed object

owner_less
provides mixed-type owner-based ordering of shared and weak pointers

*/
}


void test__smart_ptr_adaptors () //out_ptr_t, inout_ptr_t
{	PRenteredFU;

}



int main () {
	test__unique_ptr__customDeleter();
	test__unique_ptr__noFrills();
	test__unique_ptr__customDeleter();
	test__shared_ptr__aliasingCtor();
	test__enable_shared_from_this();
	test__shared_ptr__miscOther();
	test__weak_ptr();
	test__smart_ptr_adaptors();
}




#if NULL
std::atomic<std::shared_ptr> //20
  


shared_ptr<T>::operator[] //17 --- provides indexed access to the stored array
  

https://en.cppreference.com/w/cpp/memory/shared_ptr/pointer_cast
static_pointer_cast // 17 --- applies static_cast, dynamic_cast, const_cast, or reinterpret_cast to the stored pointer
dynamic_pointer_cast
const_pointer_cast
reinterpret_pointer_cast
 
std::allocate_shared, std::allocate_shared_for_overwrite
https://en.cppreference.com/w/cpp/memory/shared_ptr/allocate_shared


std::make_shared_for_overwrite
https://en.cppreference.com/w/cpp/memory/shared_ptr/make_shared


to_address (C++20) obtains a raw pointer from a pointer-like type

#endif
