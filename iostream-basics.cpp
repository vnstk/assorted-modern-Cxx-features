/*   (c)2025 Vainstein K.   */

/*
typedef  std::basic_istream<char>   std::istream;
typedef  std::basic_ostream<char>   std::ostream;

In <istream> hdr, free fu

	operator>>  --- reads values from an inp stream.

But also there's

	std::istream::operator>>(unsigned&);
	std::istream::operator>>(float&);
	......
*/

#include <string>
#include <sstream>
#include <iostream>

class Qux {
public:
	bool _bar{true};
	float _fnord{3.14159F};
};
// If you anticipate users with nondefault CharT and/or ChTraits...
template<typename CharT, typename ChTraits>
std::basic_istream<CharT,ChTraits>&
operator>> (std::basic_istream<CharT,ChTraits>& in, Qux& q)
{
	return in >> q._fnord >> q._bar;
}

class Quy {
public:
	bool _bar{true};
	float _fnord{3.14159F};
};
// If you just want clarity and concision...
std::istream&
operator>> (std::istream& in, Quy& q)
{
	return in >> q._fnord >> q._bar;
}

class Quz {
public:
	bool _bar{true};
	float _fnord{3.14159F};
	std::istream& operator>> (std::istream& in) {
		return in >> _fnord >> _bar;
	}
	std::ostream& operator<< (std::ostream& out) const {
		return out << _fnord << " , " << std::boolalpha << _bar;
	}
};
// If you're going the OO way *all* the way...


static std::string inpA{ "3.14159 true 42 hello yourself" };

int main () {
	{	std::istringstream iss(inpA);
		float f;
		bool b;
		unsigned u;
		std::string s;
		//
		iss >> f >> b >> u >> s;
		//
		std::cout << "f be: " << f << '\n';
	}

	{	std::istringstream iss(inpA);
		Qux q;
		//
		iss >> q;
		//
		std::cout << "Qux::_fnord be: " << q._fnord << '\n';
	}

	{	std::istringstream iss(inpA);
		Quy q;
		//
		iss >> q;
		//
		std::cout << "Quy::_fnord be: " << q._fnord << '\n';
	}

	{	std::istringstream iss(inpA);
		Quz q;
		//
//XXX Important: note that order of >>'s operands is reversed here! XXX
		q >> iss;
		//
//XXX Important: note that order of >>'s operands is reversed here! XXX
		q << std::cout << " --- Again note reversed order";
	}

#if 0
Equiv to
	((iss >> u) >> f) >> .....
, where
	iss >> u
means
	operator>>(istream, refToChar)
#endif

}
