#pragma once
#include <cassert>
#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

namespace convert
{

//string functions
std::string readStringFromFile(const std::string& filename);
int writeStringToFile(const std::string& str, const std::string& filename);
void replaceOneSubStringRef(std::string& s, const std::string& oldstring, const std::string& newstring, int pos0 = 0);
void replaceAllSubStringRef(std::string& s, const std::string& oldstring, const std::string& newstring);
std::string replaceOneSubString(const std::string& s, const std::string& oldstring, const std::string& newstring, int pos0 = 0);
std::string replaceAllSubString(const std::string& s, const std::string& oldstring, const std::string& newstring);
void replaceOneStringInFile(const std::string& oldfilename, const std::string& newfilename, const std::string& oldstring, const std::string& newstring);
void replaceAllStringInFile(const std::string& oldfilename, const std::string& newfilename, const std::string& oldstring, const std::string& newstring);
std::string findANumber(const std::string& s);
unsigned findTheLast(const std::string& s, const std::string& content);
std::vector<std::string> splitString(std::string str, std::string pattern = "", bool ignore_psspace = true);
bool isProChar(char c);
std::string toLowerCase(const std::string& s);
std::string toUpperCase(const std::string& s);

template <typename T>
int findNumbers(const std::string& s, std::vector<T>* data)
{
    int n = 0;
    std::string str = "";
    bool haveNum = false;
    for (int i = 0; i < s.length(); i++)
    {
        char c = s[i];
        bool findNumChar = (c >= '0' && c <= '9') || c == '.' || c == '-' || c == '+' || c == 'E' || c == 'e';
        if (findNumChar)
        {
            str += c;
            if (c >= '0' && c <= '9')
            {
                haveNum = true;
            }
        }
        if (!findNumChar || i == s.length() - 1)
        {
            if (str != "" && haveNum)
            {
                auto f = T(atof(str.c_str()));
                data->push_back(f);
                n++;
            }
            str = "";
            haveNum = false;
        }
    }
    return n;
}

template <typename T>
int findNumbers(const std::string& s, std::vector<T>& data)
{
    return findNumbers(s, &data);
}

template <typename T>
std::vector<T> findNumbers(const std::string& s)
{
    std::vector<T> data;
    findNumbers(s, &data);
    return data;
}

}    // namespace convert
