#include <Arduino.h>
#include "SplitString.h"
#include <strings.h>
#include <cctype>
#include <sstream>
#include <algorithm>
#include "utiles/debug.h"

// Función para dividir una cadena en elementos delimitados por '/'
std::vector<std::string> splitString(const std::string input, char delimiter)
{
    std::vector<std::string> elementos;
    std::istringstream stream(input);
    std::string elemento;
    while (std::getline(stream, elemento, delimiter)) elementos.push_back(elemento);
    return elementos;
}

std::vector<String> SplitString(const String& input, char delimiter)
{
    std::vector<String> elementos;
    std::istringstream stream(input.c_str());
    std::string elemento;
    while (std::getline(stream, elemento, delimiter)) elementos.push_back(elemento.c_str());
    return elementos;
}

std::string trim(const std::string& str)
{
    size_t first = str.find_first_not_of(" \t\n\r\f\v");
    if (std::string::npos == first)
    {
        return "";
    }
    size_t last = str.find_last_not_of(" \t\n\r\f\v");
    return str.substr(first, (last - first + 1));
}

// limpia una cadena del caracter ch (por ejemplo para quitarle espacios internos)
std::string cleanStr(const char* str, char ch)
{
    std::string result = str;
    result.erase(std::remove(result.begin(), result.end(), ch), result.end());
    return result;
}

std::string VectorToString(const std::vector<std::string>& vec)
{
    std::string S = "";
    for (int i = 0; i < vec.size(); i++)
    {
        if (i > 0) S += ",";
        S += vec[i];
    }
    return S;
}

std::string VectorToString(const std::vector<std::string>& vec, std::string separator)
{
    std::string S = "";
    for (int i = 0; i < vec.size(); i++)
    {
        if (i > 0) S += separator;
        S += vec[i];
    }
    return S;
}

std::string VectorToString(const std::vector<uint8_t>& vec)
{
    std::string S = "";
    for (int i = 0; i < vec.size(); i++)
    {
        if (i > 0) S += ",";
        S += std::to_string(vec[i]);
    }
    return S;
}

std::string ListToString(const std::list<std::string>& lis)
{
    std::string S = "";
    for (auto it = lis.begin(); it != lis.end(); it++)
    {
        if (it != lis.begin()) S += ",";
        S += *it;
    }
    return S;
}

std::vector<std::uint8_t> splitStringIntoBytes(const std::string input, char delimiter)
{
    std::vector<std::uint8_t> elementos;
    auto vec = splitString(input, delimiter);
    for (auto& elem : vec) elementos.push_back(std::stoi(elem));
    return elementos;
}

#include <cctype>     // std::tolower
#include <algorithm>  // std::equal

bool ichar_equals(char a, char b) { return std::tolower(static_cast<unsigned char>(a)) == std::tolower(static_cast<unsigned char>(b)); }
bool stringEquals(const std::string& a, const std::string& b) { return a.size() == b.size() && std::equal(a.begin(), a.end(), b.begin(), ichar_equals); }
// Equals sin bola a cap case!