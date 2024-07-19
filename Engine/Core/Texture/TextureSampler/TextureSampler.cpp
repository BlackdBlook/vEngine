#include "TextureSampler.h"

void TextureSampler::SetTexture(const std::string& ImageNames, bool ClearOld)
{
    throw std::runtime_error("failed to allocate vertex buffer memory!");
}

void TextureSampler::SetTexture(VkImageView ImageView, bool ClearOld)
{
    throw std::runtime_error("failed to allocate vertex buffer memory!");
}

VkImageView TextureSampler::GetImageView(uint32 FrameIndex)
{
    return VK_NULL_HANDLE;
}

void TextureSampler::SetTextureAtIndex(VkImageView ImageView, uint32 index, bool ClearOld)
{
    throw std::runtime_error("failed to allocate vertex buffer memory!");
}

void TextureSampler::CleanUp()
{
}
