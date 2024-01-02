#pragma once
#include <memory>
#include <string>
#include <unordered_map>
#include <unordered_set>


class AssetSystem
{
public:

    AssetSystem();
    
    static AssetSystem* GetInstance();
    
    std::string GetFilePathByName(const std::string& s);

    // TexturePtr LoadImage(const std::string& ImageName);

    std::string LoadTextFile(const std::string& TextName);

private:

    void InitFileMap();

    void InitIgnoreMap();
    
    void RefreshFileMap();

    bool IsIgnoreToFileMap(const std::string& Name);

    // 加载缓存
    std::unordered_map<std::string, std::weak_ptr<void>> AssetCache;
    
    // 文件映射表
    std::unordered_map<std::string, std::string> FileMap;

    // 忽略表
    std::unordered_set<std::string> IgnoreMap;
};

#define AssetSystemIns AssetSystem::GetInstance()