#pragma once
#include <functional>
#include <GLFW/glfw3.h>
#include <vulkan/vulkan_core.h>
#include "VulkanHelper/VkHelper.h"


class Level;
class VkHelper;

class vEngine 
{
    friend VkHelper;
    GLFWwindow* window;
    VkHelper vkHelper;

    SPtr<Level> CurrentLevel;
    std::vector<std::function<void()>> levelList;
    float DeltaTime = 0;

    uint64 FrameCount = 0;

    void processInput();

    void InitLevelList();
    
    void UpdateLevel();
    void DrawLevel();
    void Run();

    void DrawFrame();
    
public:
    FORCEINLINE GLFWwindow* GetWindow()
    {
        return window;
    }
    
    FORCEINLINE VkDevice GetVulkanDevice()
    {
        return vkHelper.device;
    }
    
    FORCEINLINE VkSwapchainKHR GetVkSwapchain()
    {
        return vkHelper.swapChain; 
    }
    
    FORCEINLINE VkRenderPass GetVkRenderPass()
    {
        return vkHelper.renderPass;
    }

    FORCEINLINE VkPipelineLayout GetVkPipeLineLayout()
    {
        return vkHelper.pipelineLayout;
    }

    FORCEINLINE size_t GetCurrentFrame()
    {
        return vkHelper.currentFrame;
    }

    FORCEINLINE uint64 GetFrameCount()
    {
        return FrameCount;
    }

    FORCEINLINE VkCommandPool GetCommandPool()
    {
        return vkHelper.commandPool;
    }

    FORCEINLINE VkQueue GetGraphicsQueue()
    {
        return vkHelper.graphicsQueue;
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
#define GlobalVkPipeLineLayout vEngine::ins->GetVkPipeLineLayout()
#define GFrameCount vEngine::ins->GetFrameCount()
#define GCommandPool vEngine::ins->GetCommandPool()
#define GGraphicsQueue vEngine::ins->GetGraphicsQueue()