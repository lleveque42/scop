#include "../../includes/scop.hpp"
#include <fstream>

int check_arg(int ac, char **av)
{
	ifstream obj_file;

	if (ac != 2)
		return cerr << WRONG_ARGS_NB << endl, 1;
	string file_name = av[1];
	if (file_name.size() < 5 || file_name.find(".obj", file_name.size() - 4) == string::npos)
		return cerr << WRONG_ARG_TYPE << endl, 1;
	obj_file.open(file_name);
	if (!obj_file.is_open())
		return cerr << INVALID_FILE(file_name) << endl, 1;
	obj_file.close();
	return 0;
}
