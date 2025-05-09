/*   (c)2024 Vainstein K.   */
#include "common.h"


//Jos 108

Deduction guides do not have to be templates and do not have to apply to constructors. For example,
implementing the following structure and deduction guide is valid:
template<typename T>
struct S {
T val;
};
S(const char*) -> S<std::string>; // map S<> for string literals to S<std::string>
