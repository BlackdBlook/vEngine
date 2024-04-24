#pragma once
#include <string>
#include <sstream>
class StringFormater
{
public:
    template<typename T>
    static void printer(std::stringstream& str, T arg)
    {
        print(str, arg);
    }
    
    template<typename T, typename ... TS>
    static void printer(std::stringstream& str, T arg, TS ... args)
    {
        print(str, arg);
        printer(str, args...);
    }
    
    
    template<typename ... TS>
    static std::string Formater(TS ... args)
    {
        std::stringstream str;
        printer(str, args...);
        return str.str();
    }

    template<typename T>
    static void print(std::stringstream& str, T arg)
    {
        str << arg;
    }
};

#define Format(...) StringFormater::Formater(__VA_ARGS__)