#pragma once
#include <unordered_map>
#include <vulkan/vulkan_core.h>

#include "Header.h"
#include "Engine/Core/ShaderModule/Shader.h"
#include "Engine/Core/Texture/TextureInterface/ITexture.h"
#include "ThirdParty/stb/stb_image.h"

class TexutreFile;
class TexutreFileSourceManager;


class Texture2D : public ITexture
{
    friend class DescriptorHelper;
    SPtr<TexutreFile> SourceFile;

    //Texture iamge对应的image view
    VkImageView textureImageView[MAX_FRAMES_IN_FLIGHTS] = {VK_NULL_HANDLE};

    void SetTexture_Internel(const string& TextureName);
    void cleanUp();
    void cleanUp(VkImageView* tempTextureImageView);
public:
    Texture2D(const string& TextureName);
    ~Texture2D() override;

    
    void SetTexture(const string& TextureName, bool ClearOld = true) override;
    void SetTexture(const ExternalImage& ImageView, bool ClearOld) override;
    void SetTextureAtIndex(const ExternalImage& ExternalImage, uint32 index, bool ClearOld) override;
    
    
    void CleanUp() override;
    VkImageView GetImageView(uint32 FrameIndex) override;
};
