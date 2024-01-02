#include <vulkan/vulkan_core.h>

#include "Engine/vEngine.h"
#include "LogPrinter/Log.h"

class Fence {
    VkFence handle = VK_NULL_HANDLE;
public:
    //默认构造器创建未置位的栅栏
    Fence(bool signaled = false) {
        Create(signaled);
    }
    Fence(VkFenceCreateInfo& createInfo) {
        Create(createInfo);
    }
    Fence(Fence&& other) noexcept { handle = other.handle; }
    ~Fence() { vkDestroyFence(GlobalVkLogicDevice, handle, nullptr); }
    //Getter

    //Const Function
    VkResult Wait() const {
        VkResult result = vkWaitForFences(GlobalVkLogicDevice, 1, &handle, false, UINT64_MAX);
        if (result)
            LOG("[ fence ] ERROR\nFailed to wait for the fence!\nError code: ",result,"\n");
        return result;
    }
    VkResult Reset() const {
        VkResult result = vkResetFences(GlobalVkLogicDevice, 1, &handle);
        if (result)
            LOG("[ fence ] ERROR\nFailed to reset the fence!\nError code: ",result,"\n");
        return result;
    }
    //因为“等待后立刻重置”的情形经常出现，定义此函数
    VkResult WaitAndReset() const {
        VkResult result = Wait();
        result || ((result = Reset()));
        return result;
    }
    VkResult Status() const {
        VkResult result = vkGetFenceStatus(GlobalVkLogicDevice, handle);
        if (result < 0)//vkGetFenceStatus(...)成功时有两种结果，所以不能仅仅判断result是否非0
            LOG("[ fence ] ERROR\nFailed to get the status of the fence!\nError code: ", result,"\n");
        return result;
    }
    //Non-const Function
    VkResult Create(VkFenceCreateInfo& createInfo) {
        createInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
        VkResult result = vkCreateFence(GlobalVkLogicDevice, &createInfo, nullptr, &handle);
        if (result)
            LOG("[ fence ] ERROR\nFailed to create a fence!\nError code: ", result,"\n");
        return result;
    }
    VkResult Create(bool signaled = false) {
        VkFenceCreateInfo createInfo = {};
        createInfo.flags = signaled;
        return Create(createInfo);
    }
};
