#pragma once
#include <functional>
#include <vulkan/vulkan_core.h>

#include "TypeDef.h"
#include "Core/Render/Rendering/IRendering.h"
#include "Core/Render/RenderPostProcessing/RenderPostProcessing.h"
#include "Toolkit/Container/Queue.h"
#include "VulkanHelper/VkHelper.h"


class IRendering;
class Level;
class VkHelper;

class vEngine 
{
    friend VkHelper;
    bool ExitEngine = false;
    VkHelper vkHelper;
    
    SPtr<Level> CurrentLevel;
    std::vector<std::function<void(std::shared_ptr<Level>& CurrentLevel)>>* levelList;
    float DeltaTime = 0;
    bool SwapChainRebuild = false;
    uint64 FrameCount = 0;

    bool LimtFps = false;

    void processInput();

    void InitLevelList();
    
    void UpdateLevel(float DeltaTime);
    void DrawLevel(ImGui_ImplVulkanH_Frame* fd);
    void DrawUI(ImGui_ImplVulkanH_Window* wd, ImDrawData* draw_data);
    void CombineColorAttachment();
    void FrameRender(ImGui_ImplVulkanH_Window* wd, ImDrawData* draw_data);
    void FramePresent(ImGui_ImplVulkanH_Window* wd);
    void RebuildSwapChain();
    void DrawFrameRateInfoWindow(float DeltaTime);
    void Run();
    

    std::shared_ptr<IRendering> GetRendering();
    
    
public:
    SDL_Window* GetWindow()
    {
        return vkHelper.window;
    }
    
    VkDevice GetVulkanDevice()
    {
        return vkHelper.Device;
    }
    
    VkSwapchainKHR GetVkSwapchain()
    {
        return vkHelper.MainWindowData.Swapchain; 
    }
    
    VkRenderPass GetVkRenderPass()
    {
        return vkHelper.Rendering->GetOpaqueRenderPass();
    }
    
    size_t GetCurrentFrame()
    {
        return vkHelper.MainWindowData.FrameIndex;
    }

    uint64 GetFrameCount()
    {
        return FrameCount;
    }

    VkCommandPool GetCommandPool()
    {
        return vkHelper.MainWindowData.Frames->CommandPool;
    }

    VkQueue GetGraphicsQueue()
    {
        return vkHelper.Queue;
    }
    
    static uint32 WindowX;
    static uint32 WindowY;
    vEngine();
    void SetLevel(int i);
    static vEngine* ins;
};

using Engine = vEngine;

#define GCurrentFrame vEngine::ins->GetCurrentFrame()
#define GDevice vEngine::ins->GetVulkanDevice()
#define GlobalVkSwapchain vEngine::ins->GetVkSwapchain()
#define GlobalVkRenderPass vEngine::ins->GetVkRenderPass()
#define GFrameCount vEngine::ins->GetFrameCount()
#define GCommandPool vEngine::ins->GetCommandPool()
#define GGraphicsQueue vEngine::ins->GetGraphicsQueue()