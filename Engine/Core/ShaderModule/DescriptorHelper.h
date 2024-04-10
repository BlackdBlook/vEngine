#pragma once
#include <vector>
#include <vulkan/vulkan_core.h>

class UniformBuffer;

class DescriptorHelper
{
public:
    DescriptorHelper();
    
    void createDescriptorPool();
    
    void createDescriptorSets(const VkDescriptorSetLayout& descriptorSetLayout);

    void BindMemoryBuffer(const UniformBuffer& buffer);
    
    void cleanUp();

    //存储descriptor pool，用于分配descriptor sets
    VkDescriptorPool descriptorPool;
    
    //每个运行帧的descriptor set
    std::vector<VkDescriptorSet> descriptorSets;
};
