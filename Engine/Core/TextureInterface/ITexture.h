#pragma once
#include <string>
#include "Engine/Core/ShaderModule/DescriptorHelper.h"

class ITexture
{
public:
    virtual void SetTexture(const std::string& ImageNames) = 0;
    virtual VkImageView GetImageView() = 0;
    // TODO
    ShaderTextureInput InputInfo;

    virtual ~ITexture();
};
