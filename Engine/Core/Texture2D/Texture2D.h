#pragma once
#include <unordered_map>
#include <vulkan/vulkan_core.h>

#include "Header.h"
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
    void CreateImage(VkImage& image, VkDeviceMemory& imageMemory);

    VkDeviceSize  imageSize = 0;
    int texWidth = 0;
    int texHeight = 0;
    int texChannels = 0;
    stbi_uc* pixels = nullptr;
    
    ~TexutreFile();
private:
    TexutreFile(const string& path);
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
};

class Texture2D
{
    friend class DescriptorHelper;
    SPtr<TexutreFile> SourceFile;

    //存储纹理image及其对应device memory
    VkImage textureImage = nullptr;
    VkDeviceMemory textureImageMemory = nullptr;

    //Texture iamge对应的image view
    VkImageView textureImageView;
public:
    Texture2D(const string& TextureName);
    ~Texture2D();
};
