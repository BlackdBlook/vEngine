#include "TextureCube.h"

#include <array>

#include "Engine/vEngine.h"
#include "Engine/TextureFile/TextureFile.h"
#include "Engine/VulkanHelper/VkHelper.h"

void TextureCube::SetTexture_Internel(const string& TextureName)
{
    std::array<SPtr<TextureBuffer>, 6> buffers;

    SourceFiles = NewSPtr<TextureFileArray>
    (
        std::vector<Container::Name>{
            Container::Name{"right.jpg"},   // x+
            Container::Name{"left.jpg"},    // x-
            Container::Name{"top.jpg"},     // y+
            Container::Name{"bottom.jpg"},  // y-
            Container::Name{"front.jpg"},   // z+
            Container::Name{"back.jpg"},    // z-
        }
    );

    SourceFiles->FillBuffer(buffers);
    
    auto cmd = VkHelperInstance->BeginSingleTimeCommands();

    //将texture image转换到VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL
    VkHelperInstance->transitionCubeImageLayout(cmd, SourceFiles->textureImage, VK_FORMAT_R8G8B8A8_SRGB,
        VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);

    for(int i = 0; i < 6; i++)
    {
        //执行buffer到image的copy函数
        VkBufferImageCopy region{};
        region.bufferOffset = 0;
        region.bufferRowLength = 0;
        region.bufferImageHeight = 0;

        region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        region.imageSubresource.mipLevel = 0;
        region.imageSubresource.baseArrayLayer = i;
        region.imageSubresource.layerCount = 1;

        region.imageOffset = { 0, 0, 0 };
        region.imageExtent = {
            (uint32)SourceFiles->SourceFiles[i]->texWidth,
            (uint32)SourceFiles->SourceFiles[i]->texHeight,
            1
        };

        //拷贝buffer到image
        vkCmdCopyBufferToImage(
            cmd,
            buffers[i]->stagingBuffer,
            SourceFiles->textureImage,
            VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
            1,
            &region
        );
    }

    //在copy之后进行一次transition来准备让shader访问
    VkHelperInstance->transitionCubeImageLayout(cmd, SourceFiles->textureImage, VK_FORMAT_R8G8B8A8_SRGB,
        VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
        VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
    

    VkHelperInstance->EndSingleTimeCommands(cmd);

    MAX_FRAMES_IN_FLIGHTS_LOOP(i)
    {
        textureImageView[i] = VkHelper::createImageView(
        SourceFiles->textureImage, VK_FORMAT_R8G8B8A8_SRGB,
        VK_IMAGE_ASPECT_COLOR_BIT, VK_IMAGE_VIEW_TYPE_CUBE);
    }
}

void TextureCube::cleanUp()
{
    MAX_FRAMES_IN_FLIGHTS_LOOP(CleanUpIndex)
    {
        VkImageView& target = *(textureImageView + CleanUpIndex);
        if(target != VK_NULL_HANDLE)
        {
            vkDestroyImageView(GDevice,
            target   
            , GAllocatorCallback);
            target = VK_NULL_HANDLE;
        }
    }
}

void TextureCube::cleanUp(VkImageView* tempTextureImageView)
{
    //销毁texture image view
    if(*tempTextureImageView)
    {
        vkDestroyImageView(GDevice,
            *(tempTextureImageView)
            , GAllocatorCallback);
            *(tempTextureImageView) = VK_NULL_HANDLE;
        
    }
}

TextureCube::TextureCube(const string& TextureName)
{
    SetTexture_Internel(TextureName);
}

TextureCube::~TextureCube()
{
    cleanUp();
}

void TextureCube::SetTexture(const string& TextureName, bool ClearOld)
{
    if(ClearOld)
    {
        cleanUp();
        SourceFiles.reset();
    }

    SetTexture_Internel(TextureName);
}

void TextureCube::CleanUp()
{
    cleanUp();
}

VkImageView TextureCube::GetImageView(uint32 FrameIndex)
{
    return textureImageView[FrameIndex];
}

void TextureCube::SetTexture(const ExternalImage& ImageView, bool ClearOld)
{
    if(ClearOld)
    {
        cleanUp();
        SourceFiles.reset();
    }
    
    MAX_FRAMES_IN_FLIGHTS_LOOP(i)
    {
        textureImageView[i] = VkHelper::createImageView(ImageView);
    }
}

void TextureCube::SetTextureAtIndex(const ExternalImage& ExternalImage, uint32 index, bool ClearOld)
{
    if(ClearOld)
    {
        cleanUp(textureImageView + index);
        SourceFiles.reset();
    }
    
    textureImageView[index] = VkHelper::createImageView(ExternalImage);
}
