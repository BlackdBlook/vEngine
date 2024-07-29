#pragma once
#include <SDL_video.h>
#include <stdexcept>
#include <vector>
#include <vulkan/vulkan_core.h>

#include "Header.h"
#include "backends/imgui_impl_vulkan.h"
#include "Engine/TypeDef.h"
#include "ThirdParty/StringFormater/StringFormater.h"

struct ExternalImage;
class RenderPostProcessing;
struct FrameBufferData;
class IRendering;
class TextureFileArray;
class TexutreFile;
extern class VkHelper* VkHelperInstance;
#ifdef _DEBUG 
#define APP_USE_VULKAN_DEBUG_REPORT
#endif

struct SwapChainSupportDetails {
    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> presentModes;
};

struct Attachment
{
    VkImage Image;
    VkImageView ImageView;
    VkDeviceMemory Memory;
};




//#define APP_USE_UNLIMITED_FRAME_RATE
class VkHelper
{
public:
    VkHelper();
    // Data
    VkAllocationCallbacks*   Allocator = nullptr;
    VkInstance               Instance = VK_NULL_HANDLE;
    VkPhysicalDevice         PhysicalDevice = VK_NULL_HANDLE;
    VkDevice                 Device = VK_NULL_HANDLE;
    uint32_t                 QueueFamily = (uint32_t)-1;
    VkQueue                  Queue = VK_NULL_HANDLE;
    VkDebugReportCallbackEXT DebugReport = VK_NULL_HANDLE;
    VkPipelineCache          PipelineCache = VK_NULL_HANDLE;
    VkDescriptorPool         DescriptorPool = VK_NULL_HANDLE;
    SDL_Window* window = nullptr;
    ImGui_ImplVulkanH_Window MainWindowData;
    uint32_t                 MinImageCount = MAX_FRAMES_IN_FLIGHTS;
    // Create Window Surface
    VkSurfaceKHR surface;
    VkExtent2D swapChainExtent;

    VkSampler textureSampler;

    std::shared_ptr<IRendering> Rendering = nullptr;
    SPtr<RenderPostProcessing> PostProcessing;
    
    VkSurfaceFormatKHR SwapSurfaceFormat;
    // 首先需要定义一个函数指针来获取vkDebugMarkerSetObjectNameEXT函数的地址
    PFN_vkDebugMarkerSetObjectNameEXT pfnDebugMarkerSetObjectNameEXT;

    VkPhysicalDevice SetupVulkan_SelectPhysicalDevice();
    void SetupVulkan(ImVector<const char*> instance_extensions);
    void SetupVulkanWindow(ImGui_ImplVulkanH_Window* wd, VkSurfaceKHR surface, int width, int height);
    void Init();
    // void createPostRenderPass();
    VkFormat findSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling,
                                 VkFormatFeatureFlags features);
    VkFormat findDepthAttachmentFormat();
    VkFormat findColorAttachmentFormat();
    VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);

    void createImage(uint32_t width, uint32_t height, VkFormat format,VkImageCreateFlags flags,
        VkImageTiling tiling, VkImageUsageFlags usage,
        VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory);
    void copyBufferToImage(VkCommandBuffer commandBuffer, VkBuffer buffer, VkImage image, uint32_t width,
                           uint32_t height) const;

    void createImage(TextureFileArray& file, VkImageCreateFlags flags,
                     VkImage& image, VkDeviceMemory& imageMemory);

    SwapChainSupportDetails querySwapChainSupport();
    VkExtent2D chooseSwapExtent(SwapChainSupportDetails& swapChainSupport);
    void createTextureSampler();
    void transitionImageLayout(VkCommandBuffer commandBuffer, VkImage image, VkFormat format, VkImageLayout oldLayout,
                               VkImageLayout newLayout);
    void transitionCubeImageLayout(VkCommandBuffer commandBuffer, VkImage image, VkFormat format,
                                   VkImageLayout oldLayout,
                                   VkImageLayout newLayout);
    static VkImageView createImageView(const ExternalImage& Image);
    static VkImageView createImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags, VkImageViewType viewType);
    
    uint32_t FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
    VkCommandBuffer BeginSingleTimeCommands();
    void EndSingleTimeCommands(VkCommandBuffer commandBuffer);
    void CleanupVulkanWindow();
    void CleanupVulkan();
    void cleanup();
    void WaitDeviceIdle();
    uint32 GetUniformBufferAlignment();
    uint32 GetUniformBufferOffsetByElementSize(uint32 size);
    std::shared_ptr<IRendering> CreateRendering();
    void RebuildSwapChain(bool& outNeedRebuild);
    void SetObjectMarkName(uint64 uint64, const char* c_str);

    static void check_vk_result(VkResult err)
    {
        if (err == 0)
            return;
        throw std::runtime_error(StrFormat("[vulkan] Error: VkResult = ", err));
    }
};

#define GAllocatorCallback VkHelperInstance->Allocator
#define GWindowData VkHelperInstance->MainWindowData
