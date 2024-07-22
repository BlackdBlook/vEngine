#pragma once
#include <stdexcept>
#include <unordered_map>
#include <vulkan/vulkan_core.h>

#include "Engine/TypeDef.h"
#include "Engine/Core/Container/Name.h"
#include "ThirdParty/stb/stb_image.h"

class TexutreFileSourceManager;

struct TextureBuffer
{
    
    VkBuffer stagingBuffer = nullptr;
    VkDeviceMemory stagingBufferMemory = nullptr;

    TextureBuffer(VkDeviceSize size);
    ~TextureBuffer();

    void WriteTextureSourceData(class TexutreFile* file);
};

class TexutreFile
{
    friend TexutreFileSourceManager;
public:
    SPtr<TextureBuffer> CreateMemoryBuffer();
    void CreateImage(VkImage& image, VkDeviceMemory& imageMemory, VkImageCreateFlags flags = 0);

    VkDeviceSize  imageSize = 0;
    int texWidth = 0;
    int texHeight = 0;
    int texChannels = 0;
    stbi_uc* pixels = nullptr;

    //存储纹理image及其对应device memory
    VkImage textureImage = nullptr;
    VkDeviceMemory textureImageMemory = nullptr;
    
    ~TexutreFile();
private:
    // 使用TexutreFileSourceManager::GetTextureFile来获取TexutreFile
    TexutreFile(const string& path);
};

using TexutreFilePtr = SPtr<TexutreFile>;

class TextureFileArray
{
public:
    TextureFileArray(const std::vector<Container::Name>& FileNames);
    ~TextureFileArray();
    std::vector<TexutreFilePtr> SourceFiles;

    template<size_t T>
    void FillBuffer(std::array<SPtr<TextureBuffer>, T>& buffer)
    {
        if(SourceFiles.size() == T)
        {
            for(int i = 0; i < T; i++)
            {
                buffer[i] = NewSPtr<TextureBuffer>(SourceFiles[i]->imageSize);
                buffer[i]->WriteTextureSourceData(SourceFiles[i].get());
            }
        }
        else
        {
            throw std::runtime_error("TextureFileArray::FillBuffer: SourceFiles.size() != T");
        }
    }

    int GetTexWidth();
    int GetTexHeight();
    int GetTexChannels();

    //存储纹理image及其对应device memory
    VkImage textureImage;
    VkDeviceMemory textureImageMemory;
    
    VkFormat format = VK_FORMAT_R8G8B8A8_SRGB;
    VkImageTiling tiling = VK_IMAGE_TILING_OPTIMAL;
    VkImageUsageFlags usage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
    VkMemoryPropertyFlags properties = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
};

class TexutreFileSourceManager
{
#if _DEBUG
    static std::unordered_map<string, SPtr<TexutreFile>> TextureFileCaches;
#else
    static std::unordered_map<string, WPtr<TexutreFile>> TextureFileCaches;
#endif
    
public:
    
    static SPtr<TexutreFile> GetTextureFile(const string& TextureName);

    static void Release();
};
