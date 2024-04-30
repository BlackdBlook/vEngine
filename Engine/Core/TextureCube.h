#pragma once
#include <vulkan/vulkan_core.h>

#include "Engine/TypeDef.h"
#include "ShaderModule/Shader.h"

class TexutreFile;

class TextureCube
{
public:
    friend class DescriptorHelper;
    
    std::vector<SPtr<TexutreFile>> SourceFiles;

    //存储纹理image及其对应device memory
    VkImage textureImage;
    VkDeviceMemory textureImageMemory;

    //Texture iamge对应的image view
    VkImageView textureImageView = nullptr;

    void SetTexture_Internel(const string& TextureName);
    void cleanUp(VkImageView tempTextureImageView,
    VkImage tempTextureImage, VkDeviceMemory tempTextureImageMemory);
public:
    TextureCube(const string& TextureName);
    ~TextureCube();

    // TODO
    ShaderTextureInput InputInfo;

    void SetTexture(const string& TextureName);
};