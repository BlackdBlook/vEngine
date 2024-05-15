#pragma once
#include "Engine/Core/Texture/TextureInterface/ITexture.h"


class TextureSampler : public ITexture
{
public:
    void SetTexture(const std::string& ImageNames) override;
    VkImageView GetImageView() override;
};
