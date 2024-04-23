#include "Texture2D.h"

#include <vulkan/vulkan_core.h>

#include "Engine/vEngine.h"
#include "Engine/SubSystem/AssetSystem/AssetSystem.h"
#include "Engine/Toolkit/FileToolKit/FileToolKit.h"
#include "ThirdParty/stb/stb_image.h"

void createBuffer(uint64_t imageSize, VkBufferUsageFlagBits vk_buffer_usage_flag_bits,
        int i, VkBuffer& vk_buffer, VkDeviceMemory& vk_device_memory)
{

    VkBufferCreateInfo bufferInfo{};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = imageSize;
    bufferInfo.usage = vk_buffer_usage_flag_bits;
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    if(vkCreateBuffer(GDevice, &bufferInfo, nullptr, &vk_buffer) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create buffer!");
    }
        
    VkMemoryRequirements memRequirements;
    vkGetBufferMemoryRequirements(GDevice, vk_buffer, &memRequirements);

    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = VkHelperInstance->FindMemoryType(memRequirements.memoryTypeBits,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

    if (vkAllocateMemory(GDevice, &allocInfo, nullptr, &vk_device_memory) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate vertex buffer memory!");
    }

    vkBindBufferMemory(GDevice, vk_buffer, vk_device_memory, 0);
}



void copyBufferToImage(VkCommandBuffer commandBuffer,
    VkBuffer buffer, VkImage image, uint32_t width, uint32_t height)
{
    // VkCommandBuffer commandBuffer = beginSingleTimeCommands();
    VkBufferImageCopy region{};
    region.bufferOffset = 0;
    region.bufferRowLength = 0;
    region.bufferImageHeight = 0;

    region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    region.imageSubresource.mipLevel = 0;
    region.imageSubresource.baseArrayLayer = 0;
    region.imageSubresource.layerCount = 1;

    region.imageOffset = { 0, 0, 0 };
    region.imageExtent = {
        width,
        height,
        1
    };

    //拷贝buffer到image
    vkCmdCopyBufferToImage(
        commandBuffer,
        buffer,
        image,
        VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
        1,
        &region
    );
    
    // endSingleTimeCommands(commandBuffer);
}

TextureBuffer::TextureBuffer(VkDeviceSize size)
{
    //创建staging buffer即对应的host visible memory
    createBuffer(size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
        stagingBuffer, stagingBufferMemory);
}

TextureBuffer::~TextureBuffer()
{
    //清理staging buffer和对应memory
    vkDestroyBuffer(GDevice, stagingBuffer, nullptr);
    vkFreeMemory(GDevice, stagingBufferMemory, nullptr);
}

void TextureBuffer::WriteTextureSourceData(TexutreFile* file)
{
    //将加载的图像文件像素数据拷贝到staging buffer
    void* data;
    vkMapMemory(GDevice, stagingBufferMemory, 0, file->imageSize, 0, &data);
    memcpy(data, file->pixels, static_cast<size_t>(file->imageSize));
    vkUnmapMemory(GDevice, stagingBufferMemory);
}

SPtr<TextureBuffer> TexutreFile::CreateMemoryBuffer()
{
    SPtr<TextureBuffer> buffer = NewSPtr<TextureBuffer>(imageSize);
    buffer->WriteTextureSourceData(this);

    return buffer;
}

void TexutreFile::CreateImage(VkImage& image, VkDeviceMemory& imageMemory)
{
    VkHelperInstance->createImage(texWidth, texHeight, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_TILING_OPTIMAL,
            VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
            image, imageMemory);
}

TexutreFile::TexutreFile(const string& path)
{
    //加载图像文件
    pixels = stbi_load(path.c_str(), &texWidth, &texHeight,
        &texChannels, STBI_rgb_alpha);
    imageSize = texWidth * texHeight * 4;

    if (!pixels)
    {
        throw std::runtime_error("failed to load texture image!");
    }
}

TexutreFile::~TexutreFile()
{
    //销毁原始的像素数组
    stbi_image_free(pixels);
}

#if _DEBUG
std::unordered_map<string, SPtr<TexutreFile>> TexutreFileSourceManager::TextureFileCaches;
#else
std::unordered_map<string, WPtr<TexutreFile>> TexutreFileSourceManager::TextureFileCaches;
#endif

SPtr<TexutreFile> TexutreFileSourceManager::GetTextureFile(const string& TextureName)
{
    auto it = TextureFileCaches.find(TextureName);
    if (it != TextureFileCaches.end())
    {
#if _DEBUG
        return it->second;
#else
        return it->second.lock();
#endif
    }
    else
    {
        string Path = AssetSystem::GetInstance()->GetFilePathByName(TextureName);
        TexutreFile* pFile = new TexutreFile(Path);
        SPtr<TexutreFile> File = std::shared_ptr<TexutreFile>(pFile);
        TextureFileCaches.insert({TextureName, File});
        return File;
    }
}

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
    copyBufferToImage(cmd, buffer->stagingBuffer, textureImage,
        SourceFile->texWidth, SourceFile->texHeight);

    //在copy之后进行一次transition来准备让shader访问
    VkHelperInstance->transitionImageLayout(cmd, textureImage, VK_FORMAT_R8G8B8A8_SRGB,
        VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
        VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
        

    VkHelperInstance->EndSingleTimeCommands(cmd);

    textureImageView = VkHelperInstance->createImageView(textureImage, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_ASPECT_COLOR_BIT);
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
