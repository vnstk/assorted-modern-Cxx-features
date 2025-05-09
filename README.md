# assorted-modern-Cxx-features
Trying out various modern C++ (esp. C++14 and newer) features.

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

- - - -
- - - -

Compiled with GCC 14.2.0, as
<tt>g++ -fcoroutines -fdiagnostics-path-format=inline-events -fdiagnostics-show-template-tree -fdiagnostics-text-art-charset=unicode -fno-exceptions -fno-diagnostics-show-cwe -fno-modules -fno-operator-names -grecord-gcc-switches -static -static-libgcc -static-libstdc++ -Wformat=1 -Wformat-overflow=1 -Wformat-truncation=1 -Wno-class-conversion -Wno-endif-labels -Wno-misleading-indentation -Wno-system-headers -Wno-old-style-cast -Wno-parentheses -Wno-xor-used-as-pow -Warray-bounds=1 -Wclobbered -Wdangling-else -Wdangling-reference -Wdeprecated-copy -Wduplicated-branches -Wduplicated-cond -Wfree-nonheap-object -Winfinite-recursion -Winit-self -Wmismatched-dealloc -Wmismatched-new-delete -Wmissing-include-dirs -Wmissing-template-keyword -Wmultistatement-macros -Wnoexcept-type -Wnrvo -Wodr -Woverloaded-virtual -Wpacked-not-aligned -Wpessimizing-move -Wrange-loop-construct -Wredundant-move -Wreorder -Wrestrict -Wself-move -Wsequence-point -Wshadow=local -Wsizeof-array-div -Wsizeof-pointer-div -Wstring-compare -Wstringop-overflow=2 -Wtautological-compare -Wtrigraphs -Wtype-limits -Wuse-after-free=2 -Og -std=c++</tt><i>NN</i>; where <i>NN</i> = 11|14|17|20|23|26.

In a few cases (e.g. <tt>exception-throes.cpp</tt>) omitted <tt>-fno-exceptions</tt>.
