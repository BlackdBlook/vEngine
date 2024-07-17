#pragma once
#include "Engine/Core/Material/Material.h"
#include "Engine/Core/Render/RenderPass/RenderPass.h"

class CombineSceneAndColorRenderPass
{
public:
    VkRenderPass RenderPass;
    MaterialPtr SceneColorInput;
    MaterialPtr UIColorInput;

    CombineSceneAndColorRenderPass(VkImageView SceneColor, VkImageView UIColor);

    void RenderToSurface();
private:
    void CreateRenderPass();
};
