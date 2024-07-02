#pragma once
#include <vector>
#include <string>
#include <list>

std::vector<std::uint8_t> splitStringIntoBytes(const std::string input, char delimiter);
std::vector<std::string> splitString(const std::string input, char delimiter);
std::vector<String> SplitString(const String& input, char delimiter);
std::string trim(const std::string& str);
std::string cleanStr(const char* str, char ch);

std::string ListToString(const std::list<std::string>& vec);
std::string VectorToString(const std::vector<std::string>& vec);
std::string VectorToString(const std::vector<uint8_t>& vec);
std::string VectorToString(const std::vector<std::string>& vec, std::string separator);
bool stringEquals(const std::string& a, const std::string& b);