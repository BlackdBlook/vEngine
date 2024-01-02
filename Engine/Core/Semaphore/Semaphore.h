#pragma once
#include <vulkan/vulkan_core.h>

#include "Engine/vEngine.h"
#include "LogPrinter/Log.h"

class Semaphore {
    VkSemaphore handle = VK_NULL_HANDLE;
public:
    //默认构造器创建未置位的信号量
    Semaphore() {
        Create();
    }
    Semaphore(VkSemaphoreCreateInfo& createInfo) {
        Create(createInfo);
    }
    Semaphore(Semaphore&& other) noexcept { handle = other.handle; }
    ~Semaphore() { vkDestroySemaphore(GlobalVkLogicDevice, handle, nullptr); }
    //Getter
    
    //Non-const Function
    VkResult Create(VkSemaphoreCreateInfo& createInfo) {
        createInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
        VkResult result = vkCreateSemaphore(GlobalVkLogicDevice, &createInfo, nullptr, &handle);
        if (result)
            LOG("[ semaphore ] ERROR\nFailed to create a semaphore!\nError code: ",result,"\n");
        return result;
    }
    VkResult Create() {
        VkSemaphoreCreateInfo createInfo = {};
        return Create(createInfo);
    }
};
