#pragma once

namespace utils {
	void checkArg(int ac, char **av);
	bool startsWith(const std::string str, const char *sub);
	std::vector<std::string> split(const std::string str);
}
