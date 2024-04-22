#include "DescriptorHelper.h"

#include <array>
#include <stdexcept>

#include "Header.h"
#include "Engine/vEngine.h"
#include "Engine/Core/Texture2D/Texture2D.h"
#include "Engine/Core/UniformBuffer/UniformBuffer.h"

DescriptorHelper::DescriptorHelper()
{
    createDescriptorPool();
}

DescriptorHelper::~DescriptorHelper()
{
    cleanUp();
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

void DescriptorHelper::BindInputBuffer()
{
    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHTS; i++)
    {
        int bufferCount = 0;
        std::vector<VkWriteDescriptorSet> descriptorWrites(buffers.size() + Texture2Ds.size());
        
        std::vector<VkDescriptorBufferInfo> bufferInfos(buffers.size());
        for(auto& buffer : this->buffers)
        {
            //为每个descriptor set配置其中的descriptor，每个descriptor引用一个uniform buffer
            VkDescriptorBufferInfo& bufferInfo = bufferInfos[bufferCount];
            bufferInfo.buffer = *buffer->GetUniformBuffer(i);
            bufferInfo.offset = 0;
            bufferInfo.range = buffer->GetBufferSize();
            
            //更新descriptor set
            VkWriteDescriptorSet& descriptorWrite = descriptorWrites[bufferCount++];
            descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            descriptorWrite.dstSet = descriptorSets[i];
            descriptorWrite.dstBinding = buffer->Bind;
            descriptorWrite.dstArrayElement = 0;
            descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
            descriptorWrite.descriptorCount = 1;
            descriptorWrite.pBufferInfo = &bufferInfo;
            descriptorWrite.pImageInfo = nullptr; // Optional
            descriptorWrite.pTexelBufferView = nullptr; // Optional
        }

        //在descriptor set中绑定实际的image和sampler资源到descriptors
        std::vector<VkDescriptorImageInfo> imageInfos(Texture2Ds.size());
        int imageCount = 0;
        for(auto& image : this->Texture2Ds)
        {
            auto& imageInfo = imageInfos[imageCount++];
            imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
            imageInfo.imageView = image.second->textureImageView;
            imageInfo.sampler = VkHelperInstance->textureSampler;
        
            
            VkWriteDescriptorSet& descriptorWrite = descriptorWrites[bufferCount++];
            descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            descriptorWrite.dstSet = descriptorSets[i];
            descriptorWrite.dstBinding = image.second->InputInfo.bind;
            descriptorWrite.dstArrayElement = 0;
            descriptorWrite.descriptorType = image.second->InputInfo.GetDescriptorType();
            descriptorWrite.descriptorCount = 1;
            descriptorWrite.pImageInfo = &imageInfo;
            
        }

        vkUpdateDescriptorSets(GDevice, static_cast<uint32_t>(descriptorWrites.size()),
            descriptorWrites.data(), 0, nullptr);
    }
}

const VkDescriptorSet* DescriptorHelper::GetDescriptorSetsByCurrentFrameIndex()
{
    return &descriptorSets[GCurrentFrame];
}

void DescriptorHelper::cleanUp()
{
    //销毁descriptor pool
    vkDestroyDescriptorPool(GDevice, descriptorPool, nullptr);
}
