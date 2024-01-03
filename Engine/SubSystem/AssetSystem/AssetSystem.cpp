#include "AssetSystem.h"
#include <stack>
#include "Engine/Toolkit/FileToolKit/FileToolKit.h"
#include "Engine/Toolkit/stb_img/stb_img.h"
#include "LogPrinter/Log.h"

AssetSystem::AssetSystem()
{
    InitFileMap(); 
    stbi_set_flip_vertically_on_load(false);
}

std::string AssetSystem::GetFilePathByName(const std::string& s)
{
    auto it = FileMap.find(s);
    if(it == FileMap.end())
    {
        LOG("没有找到要查找的文件", s);
        return {};
    }

    return it->second;
}

// TexturePtr AssetSystem::LoadImage(const std::string& ImageName)
// {
//     auto cache = AssetCache.find(ImageName);
//
//     // 检查缓存是否有效
//     if(cache != AssetCache.end())
//     {
//         // 缓存命中且未过期
//         if(!cache->second.expired())
//         {
//             auto ans = cache->second.lock();
//
//             return std::static_pointer_cast<m_Texture>(ans);
//         }
//     }
//     std::string Path = GetFilePathByName(ImageName);
//     
//     if(Path.empty())
//     {
//         return NULL;
//     }
//
//     TexturePtr ans = ImgToolKit::ReadImage(Path.c_str());
//     std::weak_ptr tex {ans};
//     if(cache != AssetCache.end())
//     {
//         cache->second = tex;
//     }else
//     {
//         AssetCache.insert({ImageName, tex});
//     }
//
//     return ans;
// }

std::string AssetSystem::LoadTextFile(const std::string& TextName)
{
    return "";
}

AssetSystem* AssetSystem::GetInstance()
{
    static AssetSystem* const ins = new AssetSystem();

    return ins;
}

void AssetSystem::InitFileMap()
{
    LOG("建立文件映射");
    std::string path = PathToolKit::GetCurrentPath();

    std::vector<std::string> FilePaths;
    
    PathToolKit::GetAllFileInDirectory(FilePaths, path, true);

    while(!FilePaths.empty())
    {
        std::string p;

        FilePaths.back().swap(p);

        FilePaths.pop_back();

        std::string name = FileToolKit::GetFileName(p);

        if(IsIgnoreToFileMap(name))
        {
            continue;
        }

        auto it = FileMap.find(name);

        if(it != FileMap.end() && it->first == name)
        {
            //LOG("发现重复的文件\n\t", p, "\n\t", it->second);
        }
        else
        {
            FileMap.insert({name, std::move(p)});
        }
    }
}

void AssetSystem::InitIgnoreMap()
{
    
}

void AssetSystem::RefreshFileMap()
{
    LOG("重建映射");
    FileMap.clear();
    InitFileMap();
}

bool AssetSystem::IsIgnoreToFileMap(const std::string& Name)
{
    // return IgnoreMap.find(Name) != IgnoreMap.end();
    return false;
}
