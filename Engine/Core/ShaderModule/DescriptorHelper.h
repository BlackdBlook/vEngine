#pragma once
#include <vector>
#include <vulkan/vulkan_core.h>

#include "Engine/Core/UniformBuffer/UniformBuffer.h"

class MaterialRenderPipelineInfo;
class ITexture;
class Texture2D;
class UniformBuffer;

class DescriptorHelper
{
public:
    DescriptorHelper(MaterialRenderPipelineInfo* info);

    ~DescriptorHelper();
    
    void createDescriptorPool(const std::vector<VkDescriptorPoolSize>& poolSize);
    
    void createDescriptorSets(const VkDescriptorSetLayout& descriptorSetLayout);

    void BindInputBuffer();

    const VkDescriptorSet* GetDescriptorSetsByCurrentFrameIndex();
    
    void cleanUp();

    //存储descriptor pool，用于分配descriptor sets
    VkDescriptorPool descriptorPool = VK_NULL_HANDLE;

    uint32 descriptorPoolSize = 10;
    
    //每个运行帧的descriptor set
    std::vector<VkDescriptorSet> descriptorSets;

    std::unordered_map<Container::Name, SPtr<UniformBuffer>> buffers;
    
    std::unordered_map<string, SPtr<ITexture>> Texture2Ds;
};
