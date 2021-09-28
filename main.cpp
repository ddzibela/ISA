/*
* Projekt ISA zima 2021
* @author Denis Dzíbela xdzibe00
*/
#include<iostream>
#include<string.h>
#include<getopt.h>

struct optArgs {
	bool r, s, l; //option flags
	std::string r_specifier;
	std::string s_specifier;
	optArgs() : 
		r(false),
		s(false),
		l(false)
	{}
};

/*
* @brief get commandline options and return in struct
*/
optArgs* getoptions(int argc, char** argv)
{

}

int main(int argc, char** argv)
{
	//get options
	optArgs* options = getoptions(argc, argv);
	//validate options

	//send file

	//run in server mode
	std::cout << "hello makefile\n";
	return 0;
}