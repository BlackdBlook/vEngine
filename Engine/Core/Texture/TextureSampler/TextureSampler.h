#pragma once
#include "Engine/Core/Texture/TextureInterface/ITexture.h"


class TextureSampler : public ITexture
{
public:
    void SetTexture(const std::string& ImageNames, bool ClearOld) override;
    void SetTexture(const ExternalImage& ImageView, bool ClearOld) override;
    void SetTextureAtIndex(const ExternalImage& ExternalImage, uint32 index, bool ClearOld) override;
    void CleanUp() override;
    VkImageView GetImageView(uint32 FrameIndex) override;
};
