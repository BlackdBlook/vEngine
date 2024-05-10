#pragma once
#include "Engine/Core/TextureInterface/ITexture.h"

class TextureSampler : public ITexture
{
public:
    void SetTexture(const std::string& ImageNames) override;
    VkImageView GetImageView() override;
};
