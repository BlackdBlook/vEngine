#pragma once
#include <string>
#include "Engine/Core/ShaderModule/DescriptorHelper.h"

struct ExternalImage
{
    VkImage Image;
    VkFormat Format = VK_FORMAT_R8G8B8A8_SRGB;
    VkImageAspectFlags aspectFlags = VK_IMAGE_ASPECT_COLOR_BIT;
    VkImageViewType viewType = VK_IMAGE_VIEW_TYPE_2D;

    ExternalImage(VkImage Image)
        :Image(Image)
    {
        
    }
};

class ITexture
{
public:
    virtual void SetTexture(const std::string& ImageNames, bool ClearOld = true) = 0;
    virtual void SetTexture(const ExternalImage& ImageView, bool ClearOld = true) = 0;


    virtual void SetTextureAtIndex(const ExternalImage& ExternalImage,
        uint32 index, bool ClearOld = true) = 0;

    virtual void CleanUp() = 0;
    
    virtual VkImageView GetImageView(uint32 FrameIndex) = 0;
    // TODO
    ShaderTextureInput InputInfo;

    virtual ~ITexture();
};
