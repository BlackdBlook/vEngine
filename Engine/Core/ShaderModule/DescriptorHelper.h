#pragma once
#include <vector>
#include <vulkan/vulkan_core.h>

#include "Engine/Core/UniformBuffer/UniformBuffer.h"

class Texture2D;
class UniformBuffer;

class DescriptorHelper
{
public:
    DescriptorHelper();

    ~DescriptorHelper();
    
    void createDescriptorPool();
    
    void createDescriptorSets(const VkDescriptorSetLayout& descriptorSetLayout);

    void BindMemoryBuffer(std::vector<UniformBuffer> buffers);

    void BindImageView(const Texture2D& texture, VkSampler textureSampler);

    const VkDescriptorSet* GetDescriptorSetsByCurrentFrameIndex();
    
    void cleanUp();

    //存储descriptor pool，用于分配descriptor sets
    VkDescriptorPool descriptorPool;
    
    //每个运行帧的descriptor set
    std::vector<VkDescriptorSet> descriptorSets;
    std::vector<UniformBuffer> buffers;
};
