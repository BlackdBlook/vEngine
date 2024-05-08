#pragma once
#include <unordered_map>
#include <vulkan/vulkan_core.h>

#include "Header.h"
#include "Engine/Core/ShaderModule/Shader.h"
#include "Engine/Core/TextureInterface/ITexture.h"
#include "ThirdParty/stb/stb_image.h"

class TexutreFile;
class TexutreFileSourceManager;


class Texture2D : public ITexture
{
    friend class DescriptorHelper;
    SPtr<TexutreFile> SourceFile;

    //存储纹理image及其对应device memory
    VkImage textureImage = nullptr;
    VkDeviceMemory textureImageMemory = nullptr;

    //Texture iamge对应的image view
    VkImageView textureImageView = nullptr;

    void SetTexture_Internel(const string& TextureName);
    void cleanUp(VkImageView tempTextureImageView,
    VkImage tempTextureImage, VkDeviceMemory tempTextureImageMemory);
public:
    Texture2D(const string& TextureName);
    ~Texture2D() override;



    void SetTexture(const string& TextureName) override;
    VkImageView GetImageView() override;
};
