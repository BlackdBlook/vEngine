#pragma once
#include <vulkan/vulkan_core.h>

#include "Engine/TypeDef.h"
#include "Engine/Core/Texture/TextureInterface/ITexture.h"

class TextureFileArray;
class TexutreFile;

class TextureCube : public ITexture
{
public:
    friend class DescriptorHelper;
    SPtr<TextureFileArray> SourceFiles;

    //Texture iamge对应的image view
    VkImageView textureImageView[MAX_FRAMES_IN_FLIGHTS] = {VK_NULL_HANDLE};

    void SetTexture_Internel(const string& TextureName);
    void cleanUp();
    void cleanUp(VkImageView* tempTextureImageView);
public:
    TextureCube(const string& TextureName);
    ~TextureCube() override;
    
    void SetTexture(const string& TextureName, bool ClearOld = true) override;
    void CleanUp() override;
    VkImageView GetImageView(uint32 FrameIndex) override;
    void SetTexture(const ExternalImage& ImageView, bool ClearOld) override;
    void SetTextureAtIndex(const ExternalImage& ExternalImage, uint32 index, bool ClearOld) override;
};
