#pragma once
#include "Engine/Core/Texture/TextureInterface/ITexture.h"


class TextureSampler : public ITexture
{
public:
    
    void SetTexture(const std::string& ImageNames, bool ClearOld) override;
    void SetTexture(VkImageView ImageView, bool ClearOld) override;

    VkImageView GetImageView(uint32 FrameIndex) override;
    void SetTextureAtIndex(VkImageView ImageView, uint32 index, bool ClearOld) override;
    void CleanUp() override;
};
