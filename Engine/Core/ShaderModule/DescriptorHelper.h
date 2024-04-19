#pragma once
#include <vector>
#include <vulkan/vulkan_core.h>

#include "Engine/Core/UniformBuffer/UniformBuffer.h"

class UniformBuffer;

class DescriptorHelper
{
public:
    DescriptorHelper();

    ~DescriptorHelper();
    
    void createDescriptorPool();
    
    void createDescriptorSets(const VkDescriptorSetLayout& descriptorSetLayout);

    void BindMemoryBuffer(std::vector<UniformBuffer> buffers);

    void BindImageView(VkImageView textureImageView, VkSampler textureSampler);

    const VkDescriptorSet* GetDescriptorSetsByCurrentFrameIndex();
    
    void cleanUp();

    //存储descriptor pool，用于分配descriptor sets
    VkDescriptorPool descriptorPool;
    
    //每个运行帧的descriptor set
    std::vector<VkDescriptorSet> descriptorSets;
    std::vector<UniformBuffer> buffers;
};
