#ifndef STRING_UTILS_H
#define STRING_UTILS_H

#include <string>
#include <vector>

std::vector<std::string> split(std::string string, const std::string& delimitter, int times_to_split = -1);
std::vector<std::string> split(std::string string, const std::initializer_list<std::string>& delimitters, int times_to_split = -1);
std::vector<std::string> split(std::string string, const std::vector<std::string>& delimitters, int times_to_split = -1);

#endif