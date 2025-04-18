/*   (c)2024 Vainstein K.   */
#define REPLACING_global__op_new 1
#define DEFINING_CUSTOM_global__placem_op_new 0

#include "common.h"
#include <string>
#include <vector>
#include <deque>
#include <list>
#include <set>
#include <unordered_set>

#include <memory>
#include <scoped_allocator>


#define DEF__hasMembType(membTypeName)                     \
template <typename, typename = std::void_t<>>              \
struct hasMembType__##membTypeName : std::false_type {};   \
template <typename T>                                      \
struct hasMembType__##membTypeName<T, std::void_t<typename T::membTypeName>> : std::true_type {}
//
DEF__hasMembType(allocator_type);
DEF__hasMembType(value_type);
DEF__hasMembType(pointer);
DEF__hasMembType(const_pointer);
DEF__hasMembType(void_pointer);
DEF__hasMembType(const_void_pointer);
DEF__hasMembType(difference_type);
DEF__hasMembType(size_type);
DEF__hasMembType(propagate_on_container_copy_assignment);
DEF__hasMembType(propagate_on_container_move_assignment);
DEF__hasMembType(propagate_on_container_swap);
DEF__hasMembType(is_always_equal);

#define DEF__nameOfAliasedMembType(membTypealias)                                            \
template <typename, typename = std::void_t<>>                                                \
struct nameOfAliasedMembType__##membTypealias {                                              \
static const char *c_str () { return "Absent!!"; } };                                        \
template <typename T>                                                                        \
struct nameOfAliasedMembType__##membTypealias<T, std::void_t<typename T::membTypealias>> {   \
static const char *c_str () { return typeid(typename T::membTypealias).name(); } }
//
DEF__nameOfAliasedMembType(allocator_type);
DEF__nameOfAliasedMembType(value_type);
DEF__nameOfAliasedMembType(pointer);
DEF__nameOfAliasedMembType(const_pointer);
DEF__nameOfAliasedMembType(void_pointer);
DEF__nameOfAliasedMembType(const_void_pointer);
DEF__nameOfAliasedMembType(difference_type);
DEF__nameOfAliasedMembType(size_type);
DEF__nameOfAliasedMembType(propagate_on_container_copy_assignment);
DEF__nameOfAliasedMembType(propagate_on_container_move_assignment);
DEF__nameOfAliasedMembType(propagate_on_container_swap);
DEF__nameOfAliasedMembType(is_always_equal);
DEF__nameOfAliasedMembType(rebind);

#if WHAAAA

template <typename AlloTrai>
void dumpAlloTrai (void)
{
	fuPRlit("Entered fu");
	const char *aliasedMembtypeName_mangled;
	//
	aliasedMembtypeName_mangled = nameOfAliasedMembType__value_type<AlloTrai>::c_str();
	demangleAndPRtypname(value_type,aliasedMembtypeName_mangled);
	//
	aliasedMembtypeName_mangled = nameOfAliasedMembType__pointer<AlloTrai>::c_str();
	demangleAndPRtypname(pointer,aliasedMembtypeName_mangled);
	//
	aliasedMembtypeName_mangled = nameOfAliasedMembType__const_pointer<AlloTrai>::c_str();
	demangleAndPRtypname(const_pointer,aliasedMembtypeName_mangled);
	//
	aliasedMembtypeName_mangled = nameOfAliasedMembType__void_pointer<AlloTrai>::c_str();
	demangleAndPRtypname(void_pointer,aliasedMembtypeName_mangled);
	//
	PRbool(AlloTrai::propagate_on_container_copy_assignment::value);
	PRbool(AlloTrai::propagate_on_container_move_assignment::value);
	PRbool(AlloTrai::propagate_on_container_swap::value);
	PRbool(AlloTrai::is_always_equal::value);
}


void test__learnOf_AlloTrai_membs()
{	PRenteredFU;
    std::allocator<std::string> dfltAlloForStri;
    static_assert(std::is_same_v<std::string, decltype(dfltAlloForStri)::value_type>);
 
    using dfltAlloForStri_traits = std::allocator_traits<decltype(dfltAlloForStri)>;

#if NULL
	// Turns out, they're *all* present; so that's not interesting.
	SAYevalPRret( hasMembType__propagate_on_container_copy_assignment<dfltAlloForStri_traits>::value  ,"%d");
	//
	// "St17integral_constantIbLb0EE"   ---ick!
	SAYevalPRret( nameOfAliasedMembType__propagate_on_container_copy_assignment<dfltAlloForStri_traits>::c_str()  ,"%s");
	//
	const char *aliasedMembtypeName_mangled =
	    nameOfAliasedMembType__propagate_on_container_copy_assignment<dfltAlloForStri_traits>::c_str();
	// "std::integral_constant<bool, false>"   ---not such a shining improvement, TBH.
	demangleAndPRtypname(propagate_on_container_copy_assignment,aliasedMembtypeName_mangled);
#endif

	PRbool(dfltAlloForStri_traits::propagate_on_container_copy_assignment::value);
	PRbool(dfltAlloForStri_traits::propagate_on_container_move_assignment::value);
	PRbool(dfltAlloForStri_traits::propagate_on_container_swap::value);
	PRbool(dfltAlloForStri_traits::is_always_equal::value);
	
	PRlit("now, the easy way");
	dumpAlloTrai<dfltAlloForStri_traits>();

	using striPtrTrai = std::pointer_traits<dfltAlloForStri_traits::pointer>;
}
#endif

// Adapted from https://en.cppreference.com/w/cpp/named_req/Allocator

#define custom_allo__prologue(AlloClassName)   \
template <typename T>   \
struct AlloClassName {   \
	typedef T value_type;   \
	AlloClassName() = default;   \
	template <class U> constexpr AlloClassName (const AlloClassName<U>&) noexcept {   \
		fuPRlit("Copy-ctor");   \
	}   \
	[[nodiscard]] T* allocate (const std::size_t n) {   \
		fuPRmsg("Asked to alloc count=%zu, at %zu B each; %zu B total.",   \
		        n, sizeof(T), n * sizeof(T))

#if __EXCEPTIONS
#	define custom_allo__allocate_fu__body   \
		if(n > std::size_t(-1) / sizeof(T))   \
			throw std::bad_alloc() ;   \
		if(auto p = static_cast<T*>(std::malloc(n*sizeof(T)))) return p;   \
		throw std::bad_alloc()
#else
#	define custom_allo__allocate_fu__body   \
		if(n > std::size_t(-1) / sizeof(T))   \
			assert(0);   \
		if(auto p = static_cast<T*>(std::malloc(n*sizeof(T)))) return p;   \
		assert(0);   \
	 	__builtin_unreachable ()
#endif

#define custom_allo__epilogue(AlloClassName)   \
	}   \
	void deallocate (T* p, const std::size_t) noexcept {   \
		std::free(p);   \
	}   \
};   \
template <class T, class U>   \
bool operator==(const AlloClassName<T>&, const AlloClassName<U>&) { return true; }   \
template <class T, class U>   \
bool operator!=(const AlloClassName<T>&, const AlloClassName<U>&) { return false; }   \
using flarp ## AlloClassName = int
// This last bit, the flarp##..., is just someplace to hang a semi onto.

custom_allo__prologue(Mallo);
custom_allo__allocate_fu__body;
custom_allo__epilogue(Mallo);

custom_allo__prologue(Zallo);
custom_allo__allocate_fu__body;
custom_allo__epilogue(Zallo);

custom_allo__prologue(Qallo);
custom_allo__allocate_fu__body;
custom_allo__epilogue(Qallo);


/* TODO: try Mallo with
		class-scope
		uses_allocator 

		simple ctainer
						std::vector<long, Mallo<long>>>
						std::basic_string<char, Mallo<char>>
		nested ctainer
						
						std::vector<
									std::basic_string<char, Mallo<char>>
								  , Zallo<std::basic_string<char, Mallo<char>>>
									>
*/

#if REPLACING_global__op_new
//// replace the usual global "operator new".
void* operator new  ( std::size_t count ) {
	fuPRmsg("Told to alloc %zu B.\n", count);
	return malloc(count);
}
#endif


struct Foo
{
	float                      _f;
	std::string                _s;
	std::vector<float>        _vf;
	std::vector<std::string>  _vs;
	void* operator new (std::size_t count) {
		fuPRmsg("Told to alloc %zu B; deleg to global...", count);
		return ::operator new(count);
	}
	Foo () {
//		fuPRlit("Default ctor of Foo");
	}
};

struct ThriceFoo
{
	std::vector<Foo,Zallo<Foo>> *_pvfoo;
	bool              _wellYesOrNoHuhhhhhhhhhhhhhhh;
	ThriceFoo () : _pvfoo( new std::vector<Foo,Zallo<Foo>>(3)  ) {}
};

struct Bar
{
	std::string       _sarr[3];
	void *operator new (std::size_t count, double dummyA, const char *dummyB) {
		fuPRmsg("[placem!]  Told to alloc %zu B (oh and A=%.1f B=\"%s\"); deleg to global..."
		        , count ,dummyA,dummyB);
		return ::operator new(count);
	}
//	void operator delete (void *, double, const char*){}
//	Bar(){}
};

struct Qux
{
	std::string       _sarr[3];
	Qux() {
		PRlit("Creating a Qux");
	}
};



void test__replaceGlobal_and_replaceClassScope ()
{	PRenteredFU;
	PRlit("va");
	std::vector<float> va(3);
	//
	PRlit("vc");
	std::vector<float, Zallo<float>> vc(3);
	//
	PRlit("vd");
	std::vector<Foo> vd;
	//
	PRlit("ve");
	std::vector<Foo> ve(3);
	//
	PRlit("vg");
	std::vector<Foo, Zallo<Foo>> vg(3);
	//
	PRlit("dvf5");
	std::deque<           std::vector<Foo, Zallo<Foo>> 
	           , Mallo<   std::vector<Foo, Zallo<Foo>>    >
	          > dvf5(5);
	//
	PRlit("dtf5");
	std::deque<                       ThriceFoo
	          , Mallo<                ThriceFoo>
	          > dtf5(5);
	//
	PRlit("dvtf5");
	std::deque<           std::vector<ThriceFoo
	            , Mallo<              ThriceFoo>> 
	          > dvtf5(5);
	//
	PRlit("pfoo");
	Foo *pfoo = new Foo;
	//
	PRlit("ptf0");
	ThriceFoo *ptf0 = new ThriceFoo;
	//
	PRlit("pvr");
	std::vector<Foo> *pvr = new std::vector<Foo>(3);
	//
	PRlit("pvs");
	std::vector<Foo,Mallo<Foo>> *pvs = new std::vector<Foo,Mallo<Foo>>(3);
	//
	PRlit("pdvf7");
	std::deque<           std::vector<Foo, Mallo<Foo>> 
	           , Zallo<   std::vector<Foo, Mallo<Foo>>    > >
		*pdvf7 = new
	std::deque<           std::vector<Foo, Mallo<Foo>> 
	           , Zallo<   std::vector<Foo, Mallo<Foo>>    > >
		(7);
}


#if DEFINING_CUSTOM_global__placem_op_new
void* operator new (std::size_t count, long dummyC) {
	fuPRmsg("[placem!]  Told to alloc %zu B (oh and C=%#X).\n", count, dummyC);
	return malloc(count);
}
#endif

void test__custom_placement_new()
{	PRenteredFU;
	PRlit("pa");
	Foo *pa = new Foo;

	PRlit("pb");
//	Bar *pb = new Bar; //Nnnope!  Unless we also explicitly def a Bar-specif *non-custom* op new.

	PRlit("pc");
	Bar *pc = new (3.14159,"bunnies") Bar;

	PRlit("pd");
//	Bar *pd = new (        "bunnies") Bar; // Should fail to compile
}


void test__dflt_placement_new_andRelated()
{

}


using strDeflt = std::string;
using strMallo = std::basic_string<char, std::char_traits<char>, Mallo<char>>;
using strQallo = std::basic_string<char, std::char_traits<char>, Qallo<char>>;
using strZallo = std::basic_string<char, std::char_traits<char>, Zallo<char>>;
//
template<typename T> using vecDeflt = std::vector<T>;
template<typename T> using vecMallo = std::vector<T, Mallo<T>>;
template<typename T> using vecQallo = std::vector<T, Qallo<T>>;
//
template<typename T> using setMallo = std::set<T, Mallo<T>>;
template<typename T> using setQallo = std::set<T, Qallo<T>>;
template<typename T> using setZallo = std::set<T, Zallo<T>>;
//
template<typename T> using listMallo = std::list<T, Mallo<T>>;

template<typename TOuter, typename TInner1, typename TInner2> 
using scopMQZ = std::scoped_allocator_adaptor<  Mallo<TOuter>
                                              , Qallo<TInner1>
                                              , Zallo<TInner2>
                                             >;
//
template<typename TOuter, typename TInner1>
using scopMQ  = std::scoped_allocator_adaptor<  Mallo<TOuter>
                                              , Qallo<TInner1>
                                             >;
//
template<typename TOnly>
using scopM   = std::scoped_allocator_adaptor<  Mallo<TOnly>
                                             >;
void test__multilev()
{	PRenteredFU;
	std::allocator<Mallo<std::string>> alloM_inst;
	std::allocator<Qallo<std::string>> alloQ_inst;
	std::allocator<Zallo<std::string>> alloZ_inst;
	// XXX --- yes, really *must* wrap in std::allocator (for the i/f)
	Qallo<std::string> unwrappedQ_inst;

	vecDeflt<strDeflt> v0;
	SAYeval(  v0.push_back(std::string("xxx"))   );
	SAYeval(  v0.push_back(std::string("xxx", alloQ_inst))   );
//	SAYeval(  v0.push_back(std::string("xxx", unwrappedQ_inst))   );

	vecDeflt<strMallo> v1;
//	SAYeval(  v1.push_back("xxx")   );                         //default allocator != Mallo
//	SAYeval(  v1.push_back(std::string("xxx"))   );            //default allocator != Mallo
	// This won't engage Mallo<char>, because of SSO.
	SAYeval(  v1.push_back(strMallo("xxx"))   );
	// But this will.
	SAYeval(  v1.push_back(strMallo("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"))   );
//	SAYeval(  v1.push_back(strMallo("xxx", alloQ_inst))   );   //Qallo != Mallo
//	SAYeval(  v1.push_back(strZallo("xxx", alloZ_inst))   );   //Zallo != Mallo

	vecMallo<strDeflt> v2;
	SAYeval(  v2.push_back(std::string("xxx"))   );              //uses Mallo
	SAYeval(  v2.push_back(std::string("xxx", alloQ_inst))   );  //uses Mallo too!
	SAYeval(  v2.push_back(std::string("xxx", alloZ_inst))   );  //uses Mallo too!
//	SAYeval(  v2.push_back(strQallo("xxx"))   );

	vecMallo<strMallo> v3;
	SAYeval(  v3.push_back(strMallo("xxx"))   );

	vecQallo<strMallo> v4;
	SAYeval(  v4.push_back(strMallo("xxx"))   );
//	SAYeval(  v4.push_back(strQallo("xxx"))   );          //Not on yer Nelly!

	vecMallo<setQallo<strZallo>> v5;

	std::vector<std::string, scopM<std::string>> v6;
	SAYeval(  v6.push_back("xxx")   );    // cleaner than saying "strMallo(" every time

	listMallo<vecQallo<strZallo>> listNoScop;
	SAYeval(   listNoScop.push_front(vecQallo<strZallo>(3))   );
	SAYeval(   listNoScop.front()[2].assign("zzzzz")   );
	SAYeval(   listNoScop.front()[1].assign("zzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzz")   );

	std::list< std::vector<std::unordered_set<double>>,
	           scopM< std::vector<std::unordered_set<double>> >
	         > listM;
	SAYeval(   listM.push_front(std::vector<std::unordered_set<double>>(3))   );
	SAYeval(   listM.front().resize(31415)   );
	SAYeval(   listM.front().back().insert({1,22,22,22,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,22,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,22,2,2,2,2,2,2,2,2,2,2,22,2,2,2,2,2,2,2,2,2,2,2,2,2,2,232,2,2,2,2,2,2,2,2,2,2,22,2,22,2,22,2,2})   );
//	SAYeval(   listM.front().back().insert(3.14159F)   );
	SAYeval(   listM.push_front(std::vector<std::unordered_set<double>>())   );
	SAYeval(   listM.push_front(std::vector<std::unordered_set<double>>())   );
	SAYeval(   listM.push_front(std::vector<std::unordered_set<double>>())   );
	SAYeval(   listM.push_front(std::vector<std::unordered_set<double>>())   );
	SAYeval(   listM.push_front(std::vector<std::unordered_set<double>>())   );
	SAYeval(   listM.push_front(std::vector<std::unordered_set<double>>())   );

	std::list< std::vector<std::unordered_set<double>>,
	           scopMQ< std::vector<std::unordered_set<double>> , std::unordered_set<double> >
	         > listMQ;
	SAYeval(   listMQ.push_front(std::vector<std::unordered_set<double>>(3))   );
	SAYeval(   listMQ.front().resize(31415)   );
	SAYeval(   listMQ.front().back().insert({1,22,22,22,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,22,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,22,2,2,2,2,2,2,2,2,2,2,22,2,2,2,2,2,2,2,2,2,2,2,2,2,2,232,2,2,2,2,2,2,2,2,2,2,22,2,22,2,22,2,2})   );
	PRlit("ok, done stuffing the unordered_set<double>");
//	SAYeval(   listMQ.front().back().insert(3.14159F)   );



	std::list< std::vector<std::unordered_set<double>>,
	           scopMQZ< std::vector<std::unordered_set<double>> , std::unordered_set<double> , double >
	         > listMQZ;
	SAYeval(   listMQZ.push_front(std::vector<std::unordered_set<double>>(3))   );
	SAYeval(   listMQZ.front().resize(31415)   );
	SAYeval(   listMQZ.front().back().insert({1,22,22,22,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,22,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,22,2,2,2,2,2,2,2,2,2,2,22,2,2,2,2,2,2,2,2,2,2,2,2,2,2,232,2,2,2,2,2,2,2,2,2,2,22,2,22,2,22,2,2})   );
	PRlit("ok, done stuffing the unordered_set<double>");
//	SAYeval(   listMQZ.front().back().insert(3.14159F)   );


	auto scopMQallo = listMQ.get_allocator();
//	dumpAlloTrai<decltype(scopMQallo)>();
	PRlit("...now, same but with copy-cstructed copy");
	// Must've been some obscure reason to include this copy_of_ business in std??
	auto copy_of_scopMQallo = scopMQallo.select_on_container_copy_construction();
//	dumpAlloTrai<decltype(copy_of_scopMQallo)>();
}



int main ()
{
#if REPLACING_global__op_new
	PRlit("ACHTUNG --- operator new has been replaced globally!\n\n");
	float *pf = new float;
	std::vector<float> *pvf = new std::vector<float>(11);
#endif
	SAYevalPRret(  sizeof(Foo)       ,"%zu");

	test__multilev();
return 0;

	Foo                 fOnStack;
	ThriceFoo           tfOnStack;

	test__custom_placement_new();

	test__replaceGlobal_and_replaceClassScope();

//	test__learnOf_AlloTrai_membs();


#if 0
	const char *aliasedMembtypeName_mangled = "?";
	aliasedMembtypeName_mangled = nameOfAliasedMembType__rebind<striPtrTrai>::c_str();
	demangleAndPRtypname(rebind,aliasedMembtypeName_mangled);
#endif


/*
	using allostr = std::allocator<std::string>;
	typedef std::allocator<std::string> allostr_t;

	std::allocator<std::string> al;

	if (has__is_always_equal<decltype(al)>) {
		PRtyp(decltype(al)::is_always_equal);
	}
	if (has__is_always_equal<Vallo>) {
		PRtyp(std::declval<Vallo>::is_always_equal);
	}
	prAlloTraits(al);
*/
}

/*
template<typename ALLO>
void prAlloTraits (const ALLO& allo) {
//	PRtyp(typename ALLO::allocator_type);
#if WANT_BORED
	PRtyp(typename ALLO::value_type);
	PRtyp(typename ALLO::pointer);
	PRtyp(typename ALLO::const_pointer);
#endif
//	PRtyp(typename ALLO::propagate_on_container_copy_assignment);
	PRtyp(typename ALLO::propagate_on_container_move_assignment);
//	PRtyp(typename ALLO::propagate_on_container_swap);
	PRtyp(typename ALLO::is_always_equal);
}
*/
template<typename, typename = void>
constexpr bool has__is_always_equal = false;
template<typename T>
constexpr bool has__is_always_equal<
	T,
	std::void_t<typename T::is_always_equal>
> = true;
struct Vallo {
};


//std::uses_allocator_construction_args
//std::uninitialized_construct_using_allocator
//std::make_obj_using_allocator
