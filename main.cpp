/*
* Projekt ISA zima 2021
* @author Denis Dz�bela xdzibe00
*/
#include<iostream>
#include<string.h>
#include<getopt.h>

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

/*
* @brief get commandline options and return in struct
*/
void getoptions(int argc, char** argv, optArgs* options)
{
	if (options == nullptr) {
		return;
	}

	int opt;
	while ((opt = getopt(argc, argv, "r:s:l")) != -1) {
		switch (opt) {
		case 'r':
			options->r = true;
			options->r_specifier = optarg;
			break;
		case 's':
			options->s = true;
			options->s_specifier = optarg;
			break;
		case 'l':
			options->l = true;
			break;
		case '?':
			options->u = true;
			break;
		}
	}
	//check if there are extra unknown options
	if (optind < argc) {
		options->u = true;
	}
}

int main(int argc, char** argv)
{
	//get options
	optArgs options;
	getoptions(argc, argv, &options);
	//validate options
	if (options.u) { //Unknown option provided
		std::cerr << "Unknown option provided, terminating program\n";
		return 1;
	}
	if ((options.r || options.s) && options.l) { //Incompatible options
		std::cerr << "Incompatible options selected, terminating program\n";
		return 1;
	}
	if (!(options.r && options.s) && !options.l) { //Options are missing
		std::cerr << "Incomplete option list provided, terminating program\n";
		return 1;
	}
	//no file provided
	//no address provided


	//send file

	//run in server mode
	std::cout << "hello makefile\n";
	return 0;
}