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

bool utils::startsWith(const std::string &str, const char *sub) {
	return str.find(sub, 0) == 0;
}

std::vector<std::string> utils::split(const std::string &str, const char delimiter) {
	std::string token;
	std::stringstream ss(str);
	std::vector<std::string> tokens;

	while (getline(ss, token, delimiter)) {
		if (!token.empty())
			tokens.push_back(token);
	}
	return tokens;
}

std::string utils::getWorkingDirectory() {
	char buffer[PATH_MAX];

	if (getcwd(buffer, sizeof(buffer)) != nullptr)
		return std::string(buffer);
	return "";
}
