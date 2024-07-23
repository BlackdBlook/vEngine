#include "TextureSampler.h"

void TextureSampler::SetTexture(const std::string& ImageNames, bool ClearOld)
{
    throw std::runtime_error("failed to allocate vertex buffer memory!");
}

void TextureSampler::SetTexture(const ExternalImage& ImageView, bool ClearOld)
{
}

void TextureSampler::SetTextureAtIndex(const ExternalImage& ExternalImage, uint32 index, bool ClearOld)
{
}

VkImageView TextureSampler::GetImageView(uint32 FrameIndex)
{
    return VK_NULL_HANDLE;
}

void TextureSampler::CleanUp()
{
}
