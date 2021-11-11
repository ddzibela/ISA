#pragma once
#include <string>
struct optArgs {
	bool r, s, l; //option flags
	bool u; //used for unknown option
	std::string r_specifier;
	std::string s_specifier;
	optArgs() :
		r(false),
		s(false),
		l(false),
		u(false)
	{}
};