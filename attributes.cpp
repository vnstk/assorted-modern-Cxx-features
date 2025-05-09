/*   (c)2025 Vainstein K.   */
// Expect lots of warnings when compiling this TU; that *is* the test.
#include "common.h"


#if ! __has_cpp_attribute(noreturn)		
#	error "Eh? noreturn is 11+"
#endif
[[noreturn]] void goodbyeA() {
	exit(42);
}
[[noreturn]] char goodbyeB() {
	exit(42);
	return 'Z'; //expect WARN, because fu marked [[noreturn]]
}
static char ch00 = goodbyeB(); //expect WARN


#if VER_ge14 || __has_cpp_attribute(deprecated)
[[deprecated]] constexpr unsigned m0 = 33U;
[[deprecated("just use a preprocessor macro instead")]] constexpr int m1 = 100;
//
static unsigned sta_m0 = m0; //expect WARN
#endif


#if VER_ge17

struct StruA {   char _ch{'Z'};   };
struct [[nodiscard]] StruB {   char _ch{'Z'};   };
struct [[nodiscard]] StruC : StruB {};
struct StruD : StruB {};
struct [[nodiscard]] StruE : StruA {};
//
StruB f10 () { return StruB{}; }
StruB& f11 () { static StruB x{}; return x; }
StruB *f12 () { static StruB x{}; return &x; }
StruC f13 () { return StruC{}; }
StruD f14 () { return StruD{}; }
StruE f15 () { return StruE{}; }
void f19 () {
	f10(); //expect WARN
	f11(); //expect WARN
	f12(); //expect WARN
	f13(); //expect WARN
	f14(); //expect WARN
	f15(); //expect WARN
}

//Mayn't apply to vals!//enum eUnscopedP : uint8_t { aaa=1, bbb [[nodiscard]], };
enum [[nodiscard]] eUnscopedQ : uint8_t { UQaaa=1, UQbbb, UQccc, };
//Mayn't apply to vals!//enum class eScopedP : uint8_t { aaa=1, [[nodiscard]] bbb, };
[[nodiscard]] enum class eScopedQ : uint8_t { aaa=1, bbb, ccc, };
//
//eUnscopedP f20 () { return eUnscopedP::bbb; }
eUnscopedQ f21 () { return eUnscopedQ::UQbbb; }
//eScopedP f22 () { return eScopedP::bbb; }
eScopedQ f23 () { return eScopedQ::bbb; }
void f29 () {
//	f20();//expect WARN
	f21();//expect WARN
//	f22();//expect WARN
	f23();//expect WARN
}

namespace foo {
	float *bakePi_attrBefDecl ();
};

[[nodiscard]] float *foo::bakePi_attrBefDecl () {
	return new float{3.14159F};
}

float *bakePi_attrAftFuName [[nodiscard]] () {
	return new float{3.14159F};
}

static const float sta_f0 = *bakePi_attrAftFuName();
static const float sta_f2 = *foo::bakePi_attrBefDecl();
static       float sta_f5 = [&sta_f2]() {
											*foo::bakePi_attrBefDecl();
											return 7.7F;
										}();                    //expect WARN

static float *const sta_pf2 = foo::bakePi_attrBefDecl();

#	pragma GCC diagnostic warning "-Wunused"
//XXX "maybe_unused" should've been "may_be_unused"
enum eUnscopedR : uint8_t { URaaa=1, URbbb [[maybe_unused]] , URccc, };
enum [[maybe_unused]] eUnscopedS : uint8_t { USaaa=1, USbbb, };
enum eUnscopedT : uint8_t { UTaaa=1, UTbbb, }; //expect WARN
enum class eScopedR : uint8_t { aaa=1, bbb [[maybe_unused]] , ccc, };
enum class [[maybe_unused]] eScopedS : uint8_t { aaa=1, bbb, };
enum class eScopedT : uint8_t { aaa=1, bbb, }; //expect WARN
//
eUnscopedR e20 = eUnscopedR::URaaa; //expect WARN about ccc, but not about bbb
eScopedR e21 = eScopedR::aaa; //expect WARN about ccc, but not about bbb
struct StruJ {}; //expect WARN
struct [[maybe_unused]] StruK {};
#	pragma GCC diagnostic ignored "-Wunused"

#	pragma GCC diagnostic warning "-Wimplicit-fallthrough=0"
int f30 (int x) {
	switch (x) {
		case 1: //expect WARN
		case 2:
			return 3;
		case 4: [[fallthrough]]
		case 5:
			return 6;
		default:
			return 7;
}	}
#	pragma GCC diagnostic ignored "-Wimplicit-fallthrough=0"

enum eUnscopedJ : uint8_t { UJaaa=1, UJbbb, UJccc [[deprecated]], UJddd, };
enum [[deprecated]] eUnscopedK : uint8_t { UKaaa=1, UKbbb, UKccc, UKddd, };
enum [[deprecated]] eUnscopedL : uint8_t { ULaaa=1, ULbbb [[deprecated]], }; //err?
enum class eScopedJ : uint8_t { aaa=1, bbb, ccc [[deprecated]], ddd, };
enum class [[deprecated]] eScopedK : uint8_t { aaa=1, bbb, ccc, ddd, };
//
eUnscopedJ e00;
eUnscopedJ e01{eUnscopedJ::UJbbb};
eUnscopedJ e02{eUnscopedJ::UJccc}; //expect WARN
eUnscopedK e03;                  //expect WARN
eUnscopedK e04{eUnscopedK::UKbbb}; //expect WARN
eUnscopedK e05{eUnscopedK::UKccc}; //expect WARN
//
eScopedJ e06;
eScopedJ e07{eScopedJ::bbb};
eScopedJ e08{eScopedJ::ccc}; //expect WARN
eScopedK e09;                //expect WARN
eScopedK e10{eScopedK::bbb}; //expect WARN
eScopedK e11{eScopedK::ccc}; //expect WARN

#endif // VER_ge17

//TODO
#if 0
namespace [[deprecated]] DraftAPI {

 instead of:
[[MyLib::WebService, MyLib::RestService, MyLib::doc("html")]] void foo();
you can just write
[[using MyLib: WebService, RestService, doc("html")]] void foo();
#endif



//TODO
#if 0
#if VER_ge20
[[nodiscard("reason")]]

[[no_unique_address]]

[[likely]]
[[unlikely]]
#endif

#if VER_ge23
[[assume(expression)]]
#endif

[[gnu::always_inline]] [[gnu::hot]] [[gnu::const]] [[nodiscard]]

// C++17:
[[using gnu : const, always_inline, hot]] [[nodiscard]]
#endif

int main(){}
