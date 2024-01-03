#pragma once
#include <optional>

#define VK_USE_PLATFORM_WIN32_KHR
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <vector>
#include <GLFW/glfw3native.h>


#include "Engine/TypeDef.h"
#include "Engine/Core/FrameInfo/RenderInfo.h"

class RenderPipeline;
class vEngine;

struct QueueFamilyIndices {
    std::optional<uint32_t> graphicsFamily;
    std::optional<uint32_t> presentFamily;

    bool isComplete() {
        return graphicsFamily.has_value() && presentFamily.has_value();
    }
};

struct SwapChainSupportDetails {
    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> presentModes;
};

class VkHelper
{
    vEngine* Engine;

public:
    VkDevice device;
    VkInstance Instance;
    GLFWwindow* window;
    VkDebugUtilsMessengerEXT debugMessenger;
    VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
    VkQueue graphicsQueue;
    VkQueue presentQueue;
    VkSwapchainKHR swapChain;
    std::vector<VkImage> swapChainImages;
    VkFormat swapChainImageFormat;
    VkExtent2D swapChainExtent;
    std::vector<VkImageView> swapChainImageViews;
    VkPipelineLayout pipelineLayout;
    VkSurfaceKHR surface;
    VkRenderPass renderPass;
    std::vector<VkFramebuffer> swapChainFramebuffers;
    VkCommandPool commandPool;
    VkCommandBuffer commandBuffer;

    VkSemaphore imageAvailableSemaphore;
    VkSemaphore renderFinishedSemaphore;
    VkFence inFlightFence;
private:

    VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
    SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);
    void createSwapChain();
    VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
    void createImageViews();
    void createLogicalDevice();
    void createWindowSurface();
    void CreateVkInstance();
    bool checkDeviceExtensionSupport(VkPhysicalDevice device);
    bool isDeviceSuitable(VkPhysicalDevice device);
    void pickPhysicalDevice();
    void CreateDebugUtilsMessenger();
    void createGraphicsPipeline();
    void createRenderPass();
    QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
    void createFramebuffers();
    void createCommandPool();
    void createCommandBuffer();
    void createSyncObjects();
    void recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);
public:
    VkHelper(vEngine* engine);
    void InitVulkan();
    GLFWwindow* InitWindow(uint32 X, uint32 Y);
    void CleanVk();

    void WaitDeviceIdle();
    uint32_t FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);

    RenderInfo BeginRecordCommandBuffer();
    void EndRecordCommandBuffer(const RenderInfo& RenderInfo);
};


extern VkHelper* VkHelperInstance;