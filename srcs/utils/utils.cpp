#include <fstream>
#include <sstream>

#include "scop.hpp"
#include "utils.hpp"


void utils::checkArg(int ac, char **av) {
	std::ifstream obj_file;

	if (ac != 2)
		throw std::invalid_argument(ERR_WRONG_ARGS_NB);
	std::string file_name = av[1];
	if (file_name.size() < 5 || file_name.find(".obj", file_name.size() - 4) == std::string::npos)
		throw std::invalid_argument(ERR_WRONG_ARG_TYPE);
	obj_file.open(file_name);
	if (!obj_file.is_open())
		throw std::invalid_argument(ERR_FILE_NOT_FOUND(file_name));
	obj_file.close();
}

bool utils::startsWith(const std::string str, const char *sub) {
	return str.find(sub, 0) == 0;
}

std::vector<std::string> utils::split(const std::string str) {
	std::string s;
	std::stringstream ss(str);
	std::vector<std::string> out;

	while (getline(ss, s, ' ')) {
		if (s != " ")
			out.push_back(s);
	}
	return out;
}

