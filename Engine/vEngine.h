#pragma once
#include <functional>
#include <vulkan/vulkan_core.h>

#include "TypeDef.h"
#include "Toolkit/Container/Queue.h"
#include "VulkanHelper/VkHelper.h"


class Level;
class VkHelper;

class vEngine 
{
    friend VkHelper;
    
    VkHelper vkHelper;
    
    SPtr<Level> CurrentLevel;
    std::vector<std::function<void(std::shared_ptr<Level>& CurrentLevel)>>* levelList;
    float DeltaTime = 0;
    bool SwapChainRebuild = false;
    uint64 FrameCount = 0;
    uint64 currentFrame = 0;

    void processInput();

    void InitLevelList();
    
    void UpdateLevel();
    void DrawLevel(VkCommandBuffer cmd);
    void FrameRender(ImGui_ImplVulkanH_Window* wd, ImDrawData* draw_data);
    void FramePresent(ImGui_ImplVulkanH_Window* wd);
    void RebuildSwapChain();
    void Run();

    
    
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
        return vkHelper.renderPass;
    }
    
    size_t GetCurrentFrame()
    {
        return currentFrame;
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