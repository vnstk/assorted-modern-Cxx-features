/*   (c)2024 Vainstein K.   */
#include "common.h"
#include <coroutine>

struct zPromise; // fwd decl

struct zCorouHandl : std::coroutine_handle<zPromise> //huh??????
{
	using promise_t = ::zPromise;  // in std namespc?????
};

struct zPromise
{
    zCorouHandl get_return_object() { return {zCorouHandl::from_promise(*this)}; }
    std::suspend_always initial_suspend() noexcept { return {}; }
    std::suspend_always final_suspend() noexcept { return {}; }
    void return_void() {}
    void unhandled_exception() {}
};


int main()
{
#if defined(__cpp_lib_coroutine)
PRlit("Whee, can now play with https://en.cppreference.com/w/cpp/coroutine");
#endif


	//TODO: resume
}

#if 0
https://en.cppreference.com/w/cpp/language/coroutines
https://en.cppreference.com/w/cpp/coroutine/coroutine_handle
https://en.cppreference.com/w/cpp/coroutine/generator
https://en.cppreference.com/w/cpp/coroutine/generator/promise_type/get_return_object
#endif
