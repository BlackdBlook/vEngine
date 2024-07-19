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
    auto cmd = VkHelperInstance->BeginSingleTimeCommands();

    //将texture image转换到VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL
    VkHelperInstance->transitionImageLayout(cmd, SourceFile->textureImage, VK_FORMAT_R8G8B8A8_SRGB,
        VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);

    //执行buffer到image的copy函数
    VkHelperInstance->copyBufferToImage(cmd, buffer->stagingBuffer, SourceFile->textureImage,
        SourceFile->texWidth, SourceFile->texHeight);

    //在copy之后进行一次transition来准备让shader访问
    VkHelperInstance->transitionImageLayout(cmd, SourceFile->textureImage, VK_FORMAT_R8G8B8A8_SRGB,
        VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
        VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
        

    VkHelperInstance->EndSingleTimeCommands(cmd);

    MAX_FRAMES_IN_FLIGHTS_LOOP(i)
    {
        textureImageView[i] = VkHelperInstance->createImageView(SourceFile->textureImage,
        VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_ASPECT_COLOR_BIT, VK_IMAGE_VIEW_TYPE_2D);
    }
}

void Texture2D::cleanUp()
{
    //销毁texture image view
    if(textureImageView)
    {
        MAX_FRAMES_IN_FLIGHTS_LOOP(i)
        {
            vkDestroyImageView(GDevice, *(textureImageView + i), nullptr);
            *(textureImageView + i) = VK_NULL_HANDLE;
        }
    }
}

void Texture2D::cleanUp(VkImageView* tempTextureImageView)
{
    //销毁texture image view
    if(tempTextureImageView)
    {
        vkDestroyImageView(GDevice, *tempTextureImageView, nullptr);
        *tempTextureImageView = VK_NULL_HANDLE;
    }
}

Texture2D::Texture2D(const string& TextureName)
{
    SetTexture_Internel(TextureName);
}

Texture2D::~Texture2D()
{
    cleanUp(textureImageView);
}

void Texture2D::SetTexture(VkImageView ImageView, bool ClearOld)
{
    auto tempTextureImageView = textureImageView;
    
    if(ClearOld)
    {
        cleanUp(tempTextureImageView);
    }
    MAX_FRAMES_IN_FLIGHTS_LOOP(i)
    {
        textureImageView[i] = ImageView;
    }
}

void Texture2D::SetTexture(const string& TextureName, bool ClearOld)
{
    auto tempTextureImageView = textureImageView;
    
    if(ClearOld)
    {
        cleanUp(tempTextureImageView);
    }
    
    SetTexture_Internel(TextureName);

}

void Texture2D::SetTextureAtIndex(VkImageView ImageView, uint32 index, bool ClearOld)
{
    if(ClearOld)
    {
        SourceFile.reset();
        cleanUp(textureImageView);
    }

    textureImageView[index] = ImageView;
    
}

void Texture2D::CleanUp()
{
    cleanUp();
}

VkImageView Texture2D::GetImageView(uint32 FrameIndex)
{
    return textureImageView[FrameIndex];
}
