#include "DescriptorHelper.h"

#include <stdexcept>

#include "Header.h"
#include "Engine/vEngine.h"
#include "Engine/Core/UniformBuffer/UniformBuffer.h"

DescriptorHelper::DescriptorHelper()
{
    createDescriptorPool();
}

void DescriptorHelper::createDescriptorPool()
{
    VkDescriptorPoolSize poolSize{};
    poolSize.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;//需要包含哪些descriptor类型
    poolSize.descriptorCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHTS);//数量

    VkDescriptorPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    poolInfo.poolSizeCount = 1;
    poolInfo.pPoolSizes = &poolSize;
    poolInfo.maxSets = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHTS);



    if (vkCreateDescriptorPool(GDevice, &poolInfo, nullptr, &descriptorPool) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create descriptor pool!");
    }
}

void DescriptorHelper::createDescriptorSets(
    const VkDescriptorSetLayout& descriptorSetLayout)
{
    std::vector<VkDescriptorSetLayout> layouts(MAX_FRAMES_IN_FLIGHTS, descriptorSetLayout);
    VkDescriptorSetAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocInfo.descriptorPool = descriptorPool;
    allocInfo.descriptorSetCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHTS);
    allocInfo.pSetLayouts = layouts.data();




    //为运行中的每一帧创建一个descriptor set
    descriptorSets.resize(MAX_FRAMES_IN_FLIGHTS);
    if (vkAllocateDescriptorSets(GDevice, &allocInfo, descriptorSets.data()) !=VK_SUCCESS)
    {
        throw std::runtime_error("failed to allocate descriptor sets!");
    }

}

void DescriptorHelper::BindMemoryBuffer(const UniformBuffer& buffer)
{
    
    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHTS; i++)
    {
        //为每个descriptor set配置其中的descriptor，每个descriptor引用一个uniform buffer
        VkDescriptorBufferInfo bufferInfo{};
        bufferInfo.buffer = *buffer.GetUniformBuffer(i);
        bufferInfo.offset = 0;
        bufferInfo.range = buffer.GetBufferSize();
        //更新descriptor set
        VkWriteDescriptorSet descriptorWrite{};
        descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descriptorWrite.dstSet = descriptorSets[i];
        descriptorWrite.dstBinding = 0;
        descriptorWrite.dstArrayElement = 0;
        descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        descriptorWrite.descriptorCount = 1;
        descriptorWrite.pBufferInfo = &bufferInfo;
        descriptorWrite.pImageInfo = nullptr; // Optional
        descriptorWrite.pTexelBufferView = nullptr; // Optional
        
        vkUpdateDescriptorSets(GDevice, 1, &descriptorWrite,
            0, nullptr);
    }
}

void DescriptorHelper::cleanUp()
{
    //销毁descriptor pool
    vkDestroyDescriptorPool(GDevice, descriptorPool, nullptr);
}
