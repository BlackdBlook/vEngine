#include "FileToolKit.h"
#include <fstream>
#include <filesystem>
#include <algorithm>

#include "Engine/TypeDef.h"
#include "LogPrinter/Log.h"

using namespace std;
using namespace std::filesystem;
FileDoesNotExist::FileDoesNotExist(const string&& info)
{
    this->info = info;
}

std::vector<uint8> ReadFileAsBinary_Internal(const std::string& path)
{
    ifstream fs = ifstream{path, std::ios::in | std::ios::binary | std::ios::ate};
    const size_t fileSize = (size_t) fs.tellg();
    std::vector<uint8> buffer(fileSize);

    fs.seekg(0);
    fs.read((char*)buffer.data(), (streamsize)fileSize);
    fs.close();

    return buffer;
}

string ReadFile_Internal(ifstream&& fs)
{
    if (!fs.is_open()) {
        throw std::runtime_error("failed to open file!");
    }

    
    std::string s;
    while (!fs.eof())
    {
        char c[100]={0};
        fs.read(c, 99);
        s += c;
    }
    fs.close();
    return s;
}

void FileToolKit::ToLower(string& str)
{
    std::transform(str.begin(), str.end(), str.begin(), ::tolower);
}

string FileToolKit::ReadFile(const char* path)
{
    LOG("读取文件", path);
    if (!FileExist(path))
    {
        LOG("要读取的文件不存在");
        return "";
    }

    return ReadFile_Internal(ifstream{path});
}

std::vector<uint8> FileToolKit::ReadFileAsBinary(const char* path)
{
    LOG("读取文件", path);
    if (!FileExist(path))
    {
        LOG("要读取的文件不存在");
        return {};
    }

    return ReadFileAsBinary_Internal(path);
}
 
std::string FileToolKit::ReadFile(const std::string& path)
{
    LOG(L"读取文件", path);
    if (!FileExist(path))
    {
        LOG(L"要读取的文件不存在");
        return "";
    }

    return ReadFile_Internal(ifstream{path});
}

std::vector<uint8> FileToolKit::ReadFileAsBinary(const std::string& path)
{
    LOG(L"读取文件", path);
    if (!FileExist(path))
    {
        LOG(L"要读取的文件不存在");
        return {};
    }

    return ReadFileAsBinary_Internal(path);
}

bool FileToolKit::FileExist(const char* Path)
{
    return FileToolKit::FileExist(path{Path});    
}

bool FileToolKit::FileExist(const std::string& Path)
{
    return FileToolKit::FileExist(path{Path});    
}

bool FileToolKit::FileExist(const std::filesystem::path& path)
{
    return exists(path);
}
bool FileToolKit::WriteFile(const char* path, const char* context, EWriteFileMode mode)
{
    if (mode == EWriteFileMode::CreateOrOverried)
    {
        if (FileExist(path))
        {
            RemoveFile(path);
        }
       return OverriedWrite(path,context);
    }
    else if (mode == EWriteFileMode::Append)
    {
        if (!FileExist(path))
        {
            return false;
        }
        return AppendWrite(path,context);
    }
    return false;
}

bool FileToolKit::RemoveFile(const char* path)
{
    if (!FileExist(path))
    {
        return false;
    }
    return remove(path) == 0;
}

std::string FileToolKit::GetFileName(const char* Path)
{
    return FileToolKit::GetFileName(path{Path});
}

std::string FileToolKit::GetFileName(const std::filesystem::path& Path)
{
    return Path.filename().generic_string();
}

bool FileToolKit::OverriedWrite(const char* path, const char* context)
{
    ofstream file(path, ios::out | ios::ate);
    if (!file.is_open())
        return false;
    file << context;
    file.close();
    return true;
}
bool FileToolKit::AppendWrite(const char* path, const char* context)
{
    ofstream file(path,ios::out | ios::app);
    if(!file.is_open())
        return false;
    file << context;
    file.close();
    return true;
}

string PathToolKit::GetCurrentPath()
{
    path ans;
    ans = std::filesystem::current_path();
    
    string s(ans.generic_string());

    return s;
}

string PathToolKit::GetFullPath(const char* path)
{
    return "";
}

bool PathToolKit::GetAllFileInDirectory(std::vector<std::string>& Out, const path& Path, bool SubPath)
{
    if(exists(Path) == false)
    {
        return false;
    }
    
    const directory_iterator it {Path};

    for(const auto& i : it)
    {
        if(SubPath && i.is_directory())
        {
            GetAllFileInDirectory(Out, i.path(), SubPath);
        }
        else
        {
            Out.push_back(i.path().string());
        }
    }
    return true;
}


string PathToolKit::CompiePath(vector<const char*>&& path)
{
    if (path.empty())
        return string{};
    string s;
    s += GetCurrentPath() + '\\';
    for (const char* c : path)
    {
        if (c[0] != '/' && s[s.size() - 1] != '/'
            && c[0] != '\\' && s[s.size() - 1] != '\\')
        {
            s += '\\';
        }
        s += c;
    }
    return s;
}
