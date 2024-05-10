#include "DescriptorHelper.h"

#include <array>
#include <stdexcept>

#include "Header.h"
#include "Engine/vEngine.h"
#include "Engine/Core/Material/MaterialRenderPipeline.h"
#include "Engine/Core/Texture2D/Texture2D.h"
#include "Engine/Core/UniformBuffer/UniformBuffer.h"

DescriptorHelper::DescriptorHelper(MaterialRenderPipelineInfo* info)
{
    info->MakeInputTextures(Texture2Ds);

    std::vector<VkDescriptorPoolSize> poolSize;

    info->FillVkDescriptorPoolSize(poolSize);

    createDescriptorPool(poolSize);
    
    auto setLayout = info->MakeVkDescriptorSetLayout();
    
    createDescriptorSets(setLayout);

    buffers = info->MakeUniformBuffers();

    BindInputBuffer();
}

DescriptorHelper::~DescriptorHelper()
{
    cleanUp();
}

void DescriptorHelper::createDescriptorPool(const std::vector<VkDescriptorPoolSize>& poolSize)
{
    VkDescriptorPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    poolInfo.poolSizeCount = (uint32)poolSize.size();
    poolInfo.pPoolSizes = poolSize.data();
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
    VkResult result = vkAllocateDescriptorSets(GDevice, &allocInfo, descriptorSets.data());

    if (result !=VK_SUCCESS)
    {
        throw std::runtime_error("failed to allocate descriptor sets!");
    }

}

void DescriptorHelper::BindInputBuffer()
{
    for (size_t freamIndex = 0; freamIndex < MAX_FRAMES_IN_FLIGHTS; freamIndex++)
    {
        int bufferCount = 0;
        std::vector<VkWriteDescriptorSet> descriptorWrites(buffers.size() + Texture2Ds.size());

        uint32 bufferElementCount = 0;
        for(auto& buffer : this->buffers)
        {
            bufferElementCount += buffer.second->UniformBlockCache.CaclBufferElementNum();
        }
        std::vector<VkDescriptorBufferInfo> bufferInfos(bufferElementCount);
        
        for(auto& buffer : this->buffers)
        {
            for(uint64 i = 0;i < buffer.second->UniformBlockCache.CaclBufferElementNum();i++)
            {
                auto& bufferInfo = bufferInfos[bufferCount++];
                bufferInfo.buffer = *buffer.second->GetUniformBuffer(freamIndex);
                bufferInfo.offset = i * buffer.second->UniformBlockCache.GetElementOffset();
                bufferInfo.range = buffer.second->UniformBlockCache.ElementSize;
            }
        }
        bufferCount = 0;
        bufferElementCount = 0;
        for(auto& buffer : this->buffers)
        {
            //更新descriptor set
            VkWriteDescriptorSet& descriptorWrite = descriptorWrites[bufferCount++];
            descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            descriptorWrite.dstSet = descriptorSets[freamIndex];// 目标Descriptor Set
            descriptorWrite.dstBinding = buffer.second->Bind;// 绑定点
            descriptorWrite.dstArrayElement = 0;// 数组元素的起始索引
            descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;// 描述符类型
            descriptorWrite.descriptorCount = buffer.second->UniformBlockCache.CaclBufferElementNum();// 描述符数量，对应二维数组展平后的大小
            descriptorWrite.pBufferInfo = &bufferInfos[bufferElementCount];// 指向包含数据信息的结构体的指针
            descriptorWrite.pImageInfo = nullptr; // Optional
            descriptorWrite.pTexelBufferView = nullptr; // Optional

            bufferElementCount += descriptorWrite.descriptorCount;
        }

        //在descriptor set中绑定实际的image和sampler资源到descriptors
        std::vector<VkDescriptorImageInfo> imageInfos(Texture2Ds.size());
        int imageCount = 0;
        for(auto& image : this->Texture2Ds)
        {
            auto& imageInfo = imageInfos[imageCount++];
            imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
            imageInfo.imageView = image.second->GetImageView();
            imageInfo.sampler = VkHelperInstance->textureSampler;
        
            
            VkWriteDescriptorSet& descriptorWrite = descriptorWrites[bufferCount++];
            descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            descriptorWrite.dstSet = descriptorSets[freamIndex];
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
