#include "TextureCube.h"

#include <array>

#include "Engine/vEngine.h"
#include "Engine/TextureFile/TextureFile.h"
#include "Engine/VulkanHelper/VkHelper.h"

void TextureCube::SetTexture_Internel(const string& TextureName)
{
    std::array<SPtr<TextureBuffer>, 6> buffers;

    TextureFileArray file_array
    (
        std::vector<Container::Name>{
            Container::Name{"back.jpg"},
            Container::Name{"bottom.jpg"},
            Container::Name{"front.jpg"},
            Container::Name{"left.jpg"},
            Container::Name{"right.jpg"},
            Container::Name{"top.jpg"}
        }
    );

    file_array.FillBuffer(buffers);
    
    VkHelperInstance->createImage(file_array, VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT, textureImage, textureImageMemory);

    auto cmd = VkHelperInstance->BeginSingleTimeCommands();

    //将texture image转换到VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL
    VkHelperInstance->transitionCubeImageLayout(cmd, textureImage, VK_FORMAT_R8G8B8A8_SRGB,
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
            (uint32)file_array.SourceFiles[i]->texWidth,
            (uint32)file_array.SourceFiles[i]->texHeight,
            1
        };

        //拷贝buffer到image
        vkCmdCopyBufferToImage(
            cmd,
            buffers[i]->stagingBuffer,
            textureImage,
            VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
            1,
            &region
        );
    }

    //在copy之后进行一次transition来准备让shader访问
    VkHelperInstance->transitionCubeImageLayout(cmd, textureImage, VK_FORMAT_R8G8B8A8_SRGB,
        VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
        VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
    

    VkHelperInstance->EndSingleTimeCommands(cmd);

    textureImageView = VkHelperInstance->createImageView(
        textureImage, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_ASPECT_COLOR_BIT, VK_IMAGE_VIEW_TYPE_CUBE);
}

void TextureCube::cleanUp(VkImageView tempTextureImageView, VkImage tempTextureImage,
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

TextureCube::TextureCube(const string& TextureName)
{
    SetTexture_Internel(TextureName);
}

TextureCube::~TextureCube()
{
    cleanUp(textureImageView, textureImage, textureImageMemory);
}

void TextureCube::SetTexture(const string& TextureName)
{
    auto tempTextureImageView = textureImageView;
    auto tempTextureImage = textureImage;
    auto tempTextureImageMemory = textureImageMemory;
    
    
    SetTexture_Internel(TextureName);

    cleanUp(tempTextureImageView, tempTextureImage, tempTextureImageMemory);
}

VkImageView TextureCube::GetImageView()
{
    return textureImageView;
}
