#include "Texture2D.h"

#include <vulkan/vulkan_core.h>

#include "Engine/vEngine.h"
#include "Engine/SubSystem/AssetSystem/AssetSystem.h"
#include "Engine/TextureFile/TextureFile.h"
#include "Engine/Toolkit/FileToolKit/FileToolKit.h"
#include "ThirdParty/stb/stb_image.h"

void Texture2D::SetTexture_Internel(const string& TextureName)
{
    SourceFile = TexutreFileSourceManager::GetTextureFile(TextureName);

    auto buffer = SourceFile->CreateMemoryBuffer();

    SourceFile->CreateImage(textureImage, textureImageMemory);

    
    auto cmd = VkHelperInstance->BeginSingleTimeCommands();

    //将texture image转换到VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL
    VkHelperInstance->transitionImageLayout(cmd, textureImage, VK_FORMAT_R8G8B8A8_SRGB,
        VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);

    //执行buffer到image的copy函数
    VkHelperInstance->copyBufferToImage(cmd, buffer->stagingBuffer, textureImage,
        SourceFile->texWidth, SourceFile->texHeight);

    //在copy之后进行一次transition来准备让shader访问
    VkHelperInstance->transitionImageLayout(cmd, textureImage, VK_FORMAT_R8G8B8A8_SRGB,
        VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
        VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
        

    VkHelperInstance->EndSingleTimeCommands(cmd);

    textureImageView = VkHelperInstance->createImageView(
        textureImage, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_ASPECT_COLOR_BIT, VK_IMAGE_VIEW_TYPE_2D);
}

void Texture2D::cleanUp(VkImageView tempTextureImageView, VkImage tempTextureImage,
    VkDeviceMemory tempTextureImageMemory)
{
    //销毁texture image view
    if(tempTextureImageView)
    {
        vkDestroyImageView(GDevice, tempTextureImageView, nullptr);
    }
    //销毁texture image和对应memory
    if(tempTextureImage)
    {
        vkDestroyImage(GDevice, tempTextureImage, nullptr);
    }
    if(tempTextureImageMemory)
    {
        vkFreeMemory(GDevice, tempTextureImageMemory, nullptr);
    }
}

Texture2D::Texture2D(const string& TextureName)
{
    SetTexture_Internel(TextureName);
}

Texture2D::~Texture2D()
{
    cleanUp(textureImageView, textureImage, textureImageMemory);
}

void Texture2D::SetTexture(const string& TextureName)
{
    auto tempTextureImageView = textureImageView;
    auto tempTextureImage = textureImage;
    auto tempTextureImageMemory = textureImageMemory;
    
    
    SetTexture_Internel(TextureName);

    cleanUp(tempTextureImageView, tempTextureImage, tempTextureImageMemory);
}

VkImageView Texture2D::GetImageView()
{
    return textureImageView;
}
