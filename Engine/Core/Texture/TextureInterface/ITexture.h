#pragma once
#include <string>
#include "Engine/Core/ShaderModule/DescriptorHelper.h"

class ITexture
{
public:
    virtual void SetTexture(const std::string& ImageNames, bool ClearOld = true) = 0;
    virtual void SetTexture(VkImageView ImageView, bool ClearOld = true) = 0;


    virtual void SetTextureAtIndex(VkImageView ImageView,
        uint32 index, bool ClearOld = true) = 0;

    virtual void CleanUp() = 0;
    
    virtual VkImageView GetImageView(uint32 FrameIndex) = 0;
    // TODO
    ShaderTextureInput InputInfo;

    virtual ~ITexture();
};
