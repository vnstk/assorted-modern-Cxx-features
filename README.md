# assorted-modern-Cxx-features
Trying out various modern C++ (esp. C++14 and newer) features.

- - - -
- - - -
- - - -

A noteworthy weirdness:
```
void fone (Foo const& r) {
	static_assert(std::is_const<         std::remove_reference<decltype(r)>::type>::value);
}
```
is fine, but
```
template<typename T>
void ftwo (T const& r) {
	static_assert(std::is_const<         std::remove_reference<decltype(r)>::type>::value);
}
```
gives compiletime error (not a "static assert is false" error, but a "cannot compile your program" error).&nbsp;&nbsp;The rather unobvious workaround is
```
template<typename T>
void ftwo (T const& r) {
	static_assert(std::is_const<typename std::remove_reference<decltype(r)>::type>::value);
}
```
But wait, it gets more weird!  The said workaround is also accepted in the <i>non</i>-templated function,
```
void fone (Foo const& r) {
	static_assert(std::is_const<typename std::remove_reference<decltype(r)>::type>::value);
}
```
