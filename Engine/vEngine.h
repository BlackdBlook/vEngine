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

    void processInput();

    void InitLevelList();
    
    void UpdateLevel();
    void DrawLevel();
    void Run();
public:
    GLFWwindow* GetWindow()
    {
        return window;
    }
    
    VkDevice GetVulkanDevice()
    {
        return vkHelper.device;
    }
    
    VkSwapchainKHR GetVkSwapchain()
    {
        return vkHelper.swapChain; 
    }
    
    VkRenderPass GetVkRenderPass()
    {
        return vkHelper.renderPass;
    }
    
    static constexpr uint32 WindowX = 2560;
    static constexpr uint32 WindowY = 1440;
    vEngine();
    void SetLevel(int i);
    static vEngine* ins;
};


#define GlobalVkLogicDevice vEngine::ins->GetVulkanDevice()
#define GlobalVkSwapchain vEngine::ins->GetVkSwapchain()
#define GlobalVkRenderPass vEngine::ins->GetVkRenderPass()