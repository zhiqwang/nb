#pragma once
#include <cassert>
#include <cstdlib>
#include <stdexcept>
#include <string>
#include <vector>

class convert
{
public:
    //string functions
    static std::string readStringFromFile(const std::string& filename);
    static int writeStringToFile(const std::string& str, const std::string& filename);
    static void writeStringAppendToFile(const std::string& str, FILE* fp);
    static void replaceOneSubStringRef(std::string& s, const std::string& oldstring, const std::string& newstring, int pos0 = 0);
    static void replaceAllSubStringRef(std::string& s, const std::string& oldstring, const std::string& newstring);
    static std::string replaceOneSubString(const std::string& s, const std::string& oldstring, const std::string& newstring, int pos0 = 0);
    static std::string replaceAllSubString(const std::string& s, const std::string& oldstring, const std::string& newstring);
    static void replaceOneStringInFile(const std::string& oldfilename, const std::string& newfilename, const std::string& oldstring, const std::string& newstring);
    static void replaceAllStringInFile(const std::string& oldfilename, const std::string& newfilename, const std::string& oldstring, const std::string& newstring);
    static std::string findANumber(const std::string& s);
    static unsigned findTheLast(const std::string& s, const std::string& content);
    static std::vector<std::string> splitString(std::string str, std::string pattern = "", bool ignore_psspace = true);
    static bool isProChar(char c);
    static std::string toLowerCase(const std::string& s);
    static std::string toUpperCase(const std::string& s);

    template <typename T>
    static int findNumbers(const std::string& s, std::vector<T>* data)
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
    static int findNumbers(const std::string& s, std::vector<T>& data)
    {
        return findNumbers(s, &data);
    }

    template <typename T>
    static std::vector<T> findNumbers(const std::string& s)
    {
        std::vector<T> data;
        findNumbers(s, &data);
        return data;
    }

private:
    static void check_format1(std::vector<std::string>& strs, int i) { }
    template <typename T, typename... Args>
    static void check_format1(std::vector<std::string>& strs, int i, T t, Args... args)
    {
        switch (strs[i].back())
        {
        case 's':
            if (std::is_same<const char*, T>::value || std::is_same<char*, T>::value)
            {

            }
            else
            {
                auto error_str = std::string("need %s, but ") + typeid(T).name() + " supplied!";
                std::cerr << error_str << std::endl;
                throw std::runtime_error(error_str);
            }
        }
        check_format1(strs, i + 1, args...);
    }
public:
    template <typename... Args>
    static void checkFormatStr(const std::string& format_str, Args... args)
    {
        std::vector<std::string> format_strs;
        size_t p0 = 0, p1 = 0, p2 = 0;
        while (p1 != std::string::npos)
        {
            p1 = format_str.find_first_of("%", p1);
            if (p1 != std::string::npos)
            {
                p2 = format_str.find_first_of("diuoxXfFeEgGaAcspn", p1 + 1);
                if (p2 != std::string::npos)
                {
                    if (format_str.substr(p1 + 1, p2 - p1 - 1).find_first_not_of("0123456789.+-*#") == std::string::npos)
                    {
                        format_strs.push_back(format_str.substr(p1, p2 - p1 + 1));
                        p0 = p2 + 1;
                    }
                }
                p1++;
            }
        }
        if (format_strs.size() != sizeof...(args))
        {
            auto error_str = "need " + std::to_string(format_strs.size()) + " parameters, but " + std::to_string(sizeof...(args)) + " supplied!";
            std::cerr << error_str << std::endl;
            throw std::runtime_error(error_str);
        }
        else
        {
            check_format1(format_strs, 0, args...);
        }
    }

    template <typename... Args>
    static void formatAppendString(std::string& str, const char* format_str, Args... args)
    {
#ifdef _DEBUG
        checkFormatStr(format_str, args...);
#endif
        char c[1024];
        int len = snprintf(c, sizeof(c), format_str, args...);
        std::string s(c);
        if (len >= sizeof(c))
        {
            auto c1 = new char[len + 1];
            snprintf(c1, len, format_str, args...);
            s = c1;
            delete c1;
        }
        str += std::string(s);
    }

    template <typename... Args>
    static std::string formatString(Args... args)
    {
        std::string s;
        formatAppendString(s, args...);
        return s;
    }
};
