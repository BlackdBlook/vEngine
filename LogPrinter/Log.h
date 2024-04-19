#pragma once
#include <chrono>
#include <iostream>
#include <vector>
#include <glm/fwd.hpp>


std::ostream &operator<<(std::ostream &output, const glm::vec2 &D );
std::ostream &operator<<(std::ostream &output, const glm::vec3 &D );
std::ostream &operator<<(std::ostream &output, const glm::vec4 &D );
std::ostream &operator<<(std::ostream &output, const glm::mat4 &D );
std::ostream &operator<<(std::ostream &output, const std::chrono::microseconds &D );
std::ostream &operator<<(std::ostream &output, const glm::quat &D );
std::ostream &operator<<(std::ostream &output, const std::vector<std::string> &D );

constexpr static const char* get_filename(const char* path) {
    const char* file = path;
    while (*path) {
        char c = *path++;
        if (c == '/' || c == '\\') {
            file = path;
        }
    }
    return file;
}

class Log 
{
public:
    
    template<typename T>
    static void printer(T arg)
    {
        print(arg);
        std::cout << std::endl;
    }
    
    template<typename T, typename ... TS>
    static void printer(T arg, TS ... args)
    {
        print(arg);
        printer(args...);
    }

    template<typename ... TS>
    static void NameSpacePrinterLog(const char* FileName, int Line, TS ... args)
    {
        std::cout << "\033[43m";
        std::cout << FileName << " Line:" << Line ;
        std::cout << ' ' << '\n';
        std::cout << "\033[0m";
        std::cout << '\t';
        printer(args...);
    }

    template<typename ... TS>
    static void NameSpacePrinterWaring(const char* FileName, int Line, TS ... args)
    {
        std::cout << "\033[44m";
        std::cout << FileName << " Line:" << Line ;
        std::cout << ' ' << '\n';
        std::cout << "\033[0m";
        std::cout << '\t';
        printer(args...);
    }

    template<typename ... TS>
    static void NameSpacePrinterError(const char* FileName, int Line, TS ... args)
    {
        std::cout << "\033[43m";
        std::cout << FileName << " Line:" << Line ;
        std::cout << ' ' << '\n';
        std::cout << "\033[0m";
        std::cout << '\t';
        printer(args...);
    }
    

    template<typename T>
    static void print(T arg)
    {
        std::cout << arg << ' ';
    }
    
    template<>
    static void print<bool>(bool arg);
};

#define LOG(...) Log::NameSpacePrinterLog(get_filename(__FILE__), __LINE__, __VA_ARGS__)
#define WARING(...) Log::NameSpacePrinterWaring(get_filename(__FILE__), __LINE__, __VA_ARGS__)
#define ERR(...) Log::NameSpacePrinterError(get_filename(__FILE__), __LINE__, __VA_ARGS__)