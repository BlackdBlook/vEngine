#pragma once
#include <string>
#include <exception>
#include <filesystem>
#include <vector>

#include "Engine/TypeDef.h"

class FileDoesNotExist : public std::exception
{
public:
    FileDoesNotExist(const std::string&& info);
    std::string info;
};
enum class EWriteFileMode : unsigned char
{
    CreateOrOverried = 0,
    Append
};
class FileToolKit
{
public:

    static void ToLower(string& str);

    static std::string ReadFile(const char* path);

    static std::string ReadFile(const std::string& path);
    
    static std::vector<uint8> ReadFileAsBinary(const char* path);

    static std::vector<uint8> ReadFileAsBinary(const std::string& path);

    static bool FileExist(const char* path);
    
    static bool FileExist(const std::string& path);
    
    static bool FileExist(const std::filesystem::path& path);

    static bool WriteFile(const char* path, const char* context, EWriteFileMode mode);

    static bool RemoveFile(const char* path);

    static std::string GetFileName(const char* Path);
    
    static std::string GetFileName(const std::filesystem::path& Path);
private:
    FileToolKit(){}

    static bool OverriedWrite(const char* path, const char* context);

    static bool AppendWrite(const char* path, const char* context);
};
class PathToolKit
{
public:

    static std::string GetCurrentPath();

    static std::string GetFullPath(const char* path);

    static bool GetAllFileInDirectory(std::vector<std::string>& Out, const std::filesystem::path& Path, bool SubPath = false);
    
    static std::string CompiePath(std::vector<const char*>&& path);
private:

};

