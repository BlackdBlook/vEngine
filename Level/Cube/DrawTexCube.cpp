#include "DrawTexCube.h"

#include <stdexcept>

#include "Engine/vEngine.h"
#include "Engine/Core/Component/Component.h"
#include "Engine/Core/Material/Material.h"
#include "Engine/Core/Object/Object.h"
#include "Meshs/Box/BoxVertices.h"
#include "ThirdParty/stb/stb_image.h"

namespace
{
    struct UniformBufferObject
    {
        glm::mat4 model;
        glm::mat4 view;
        glm::mat4 proj;
    };
    
    class TexCube : public Component
    {
        SPtr<Material> material;

        MeshVertexBuffer buffer;
    public:
        TexCube();

        ~TexCube() override;

        virtual void Update(float DeltaTime) override;

        virtual void Draw(const RenderInfo& RenderInfo) override; 
    };

    
    //存储纹理image及其对应device memory
    VkImage textureImage;
    VkDeviceMemory textureImageMemory;
    //Texture iamge对应的image view
    VkImageView textureImageView;
    
    void createBuffer(uint64_t imageSize, VkBufferUsageFlagBits vk_buffer_usage_flag_bits,
        int i, VkBuffer& vk_buffer, VkDeviceMemory& vk_device_memory)
    {

        VkBufferCreateInfo bufferInfo{};
        bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        bufferInfo.size = imageSize;
        bufferInfo.usage = vk_buffer_usage_flag_bits;
        bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

        if(vkCreateBuffer(GDevice, &bufferInfo, nullptr, &vk_buffer) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to create buffer!");
        }
        
        VkMemoryRequirements memRequirements;
        vkGetBufferMemoryRequirements(GDevice, vk_buffer, &memRequirements);

        VkMemoryAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        allocInfo.allocationSize = memRequirements.size;
        allocInfo.memoryTypeIndex = VkHelperInstance->FindMemoryType(memRequirements.memoryTypeBits,
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

        if (vkAllocateMemory(GDevice, &allocInfo, nullptr, &vk_device_memory) != VK_SUCCESS) {
            throw std::runtime_error("failed to allocate vertex buffer memory!");
        }

        vkBindBufferMemory(GDevice, vk_buffer, vk_device_memory, 0);
    }

    void createImage(uint32_t width, uint32_t height, VkFormat format,
        VkImageTiling tiling,VkImageUsageFlags usage, VkMemoryPropertyFlags properties,
        VkImage& image, VkDeviceMemory& imageMemory)
    {
        //创建image
        VkImageCreateInfo imageInfo{};
        imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
        imageInfo.imageType = VK_IMAGE_TYPE_2D;
        imageInfo.extent.width = width;
        imageInfo.extent.height = height;
        imageInfo.extent.depth = 1;
        imageInfo.mipLevels = 1;
        imageInfo.arrayLayers = 1;
        imageInfo.format = format;
        imageInfo.tiling = tiling;
        imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;//GPU无法使用，但是第一个transition将保留texels
        imageInfo.usage = usage;
        imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
        imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

        if (vkCreateImage(GDevice, &imageInfo, nullptr, &image) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to create image!");
        }

        //获取image对应的内存要求（找到对应的内存类型）
        VkMemoryRequirements memRequirements;
        vkGetImageMemoryRequirements(GDevice, image, &memRequirements);
        //分配内存
        VkMemoryAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        allocInfo.allocationSize = memRequirements.size;
        allocInfo.memoryTypeIndex = VkHelperInstance->FindMemoryType(memRequirements.memoryTypeBits,
            properties);

        if (vkAllocateMemory(GDevice, &allocInfo, nullptr, &imageMemory) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to allocate image memory!");
        }
        
        //绑定image和为其分配的实际内存
        vkBindImageMemory(GDevice, image, imageMemory, 0);
    }
    
    //抽象出Single time command buffer的录制与结束
    VkCommandBuffer beginSingleTimeCommands()
    {
        VkCommandBufferAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandPool = Engine::ins->GetCommandPool();
        allocInfo.commandBufferCount = 1;

        VkCommandBuffer commandBuffer;
        vkAllocateCommandBuffers(GDevice, &allocInfo, &commandBuffer);

        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

        vkBeginCommandBuffer(commandBuffer, &beginInfo);

        return commandBuffer;
    }


    void endSingleTimeCommands(VkCommandBuffer commandBuffer)
    {
        vkEndCommandBuffer(commandBuffer);

        VkSubmitInfo submitInfo{};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &commandBuffer;

        vkQueueSubmit(GGraphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
        vkQueueWaitIdle(GGraphicsQueue);

        vkFreeCommandBuffers(GDevice, GCommandPool, 1, &commandBuffer);
    }
    
    //将一个buffer的内容拷贝到另一个buffer
    void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size)
    {
        //创建一个临时的command buffer用于内存传输操作
        VkCommandBuffer commandBuffer = beginSingleTimeCommands();

        //拷贝
        VkBufferCopy copyRegion{};
        copyRegion.srcOffset = 0;// Optional
        copyRegion.dstOffset = 0;// Optional
        copyRegion.size = size;
        vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

        endSingleTimeCommands(commandBuffer);
    }

    //处理layout转换
    void transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout)
    {
        VkCommandBuffer commandBuffer = beginSingleTimeCommands();

        VkPipelineStageFlags sourceStage;
        VkPipelineStageFlags destinationStage;

        VkImageMemoryBarrier barrier{};
        barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
        barrier.oldLayout = oldLayout;
        barrier.newLayout = newLayout;
    
        barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;

        barrier.image = image;
        barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        barrier.subresourceRange.baseMipLevel = 0;
        barrier.subresourceRange.levelCount = 1;
        barrier.subresourceRange.baseArrayLayer = 0;
        barrier.subresourceRange.layerCount = 1;
        
        if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL)
        {
            barrier.srcAccessMask = 0;//write不需要等待任何事情
            barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;//transfer write必须发生在pipeline transfer stage

            sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;//最早的可能的pipeline stage
            destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
        }
        else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL)
        {
            barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
            barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;//image将在同一pipeline stage写入，随后由片元着色器读取

            sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
            destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
        }
        else
        {
            throw std::invalid_argument("unsupported layout transition!");
        }

        vkCmdPipelineBarrier(commandBuffer, 
            sourceStage, destinationStage, 
            0,
            0, nullptr,
            0, nullptr,
            1, &barrier);

        endSingleTimeCommands(commandBuffer);
    }

    void copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height)
    {
        VkCommandBuffer commandBuffer = beginSingleTimeCommands();
        VkBufferImageCopy region{};
        region.bufferOffset = 0;
        region.bufferRowLength = 0;
        region.bufferImageHeight = 0;

        region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        region.imageSubresource.mipLevel = 0;
        region.imageSubresource.baseArrayLayer = 0;
        region.imageSubresource.layerCount = 1;

        region.imageOffset = { 0, 0, 0 };
        region.imageExtent = {
            width,
            height,
            1
        };

        //拷贝buffer到image
        vkCmdCopyBufferToImage(
            commandBuffer,
            buffer,
            image,
            VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
            1,
            &region
        );

        
        endSingleTimeCommands(commandBuffer);
    }

    void createTexture()
    {
        //加载图像文件
        int texWidth, texHeight, texChannels;
        stbi_uc* pixels = stbi_load("F:\\Code\\vEngine\\Content\\container2.png", &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
        VkDeviceSize  imageSize = texWidth * texHeight * 4;

        if (!pixels)
        {
            throw std::runtime_error("failed to load texture image!");
        }

        //创建staging buffer即对应的host visible memory
        VkBuffer stagingBuffer = nullptr;
        VkDeviceMemory stagingBufferMemory = nullptr;
        createBuffer(imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
            stagingBuffer, stagingBufferMemory);

        //将加载的图像文件像素数据拷贝到staging buffer
        void* data;
        vkMapMemory(GDevice, stagingBufferMemory, 0, imageSize, 0, &data);
        memcpy(data, pixels, static_cast<size_t>(imageSize));
        vkUnmapMemory(GDevice, stagingBufferMemory);

        //销毁原始的像素数组
        stbi_image_free(pixels);
        //创建textureImage及其内存
        createImage(texWidth, texHeight, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_TILING_OPTIMAL,
            VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
            textureImage, textureImageMemory);

        //将texture image转换到VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL
        transitionImageLayout(textureImage, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
        //执行buffer到image的copy函数
        copyBufferToImage(stagingBuffer, textureImage, static_cast<uint32_t>(texWidth), static_cast<uint32_t>(texHeight));

        //在copy之后进行一次transition来准备让shader访问
        transitionImageLayout(textureImage, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

        //清理staging buffer和对应memory
        vkDestroyBuffer(GDevice, stagingBuffer, nullptr);
        vkFreeMemory(GDevice, stagingBufferMemory, nullptr);
    }

    void cleanup()
    {
        //销毁texture image view
        vkDestroyImageView(GDevice, textureImageView, nullptr);
        
        //销毁texture image和对应memory
        vkDestroyImage(GDevice, textureImage, nullptr);
        vkFreeMemory(GDevice, textureImageMemory, nullptr);
    }

    void createTextureImageView()
    {
        textureImageView = VkHelperInstance->createImageView(textureImage, VK_FORMAT_R8G8B8A8_SRGB);
    }
    
    class MaterialRenderPipelineInfoTexCube : public MaterialRenderPipelineInfo
    {
    public:
        MaterialRenderPipelineInfoTexCube(const string& shaderName, ShaderCodeType codeType = ShaderCodeType::HLSL);
        VkDescriptorSetLayout MakeVkDescriptorSetLayout() const override;
    };
    
}


TexCube::TexCube() :
    buffer(sizeof(BoxVertices), BoxVertices)
{
    UniformBufferObject ubo{};

    {
        // MaterialRenderPipelineInfoTexCube info;

        createTexture();
        createTextureImageView();

        SPtr<MaterialRenderPipelineInfo> info = NewSPtr<MaterialRenderPipelineInfoTexCube>("DrawTexCube");
     
        material = std::make_shared<Material>(info);

        material->descriptor.BindImageView(textureImageView, VkHelperInstance->textureSampler);
    }
    
    ubo.view = glm::lookAt(glm::vec3(0.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

    ubo.proj = glm::perspective(glm::radians(90.0f), Engine::ins->WindowX / (float)Engine::ins->WindowY, 1.f, 10.0f);

    material->SetAllUniformData("type.ConstantBuffer.UBO", "model", MAT4());
    material->SetAllUniformData("type.ConstantBuffer.UBO", "u_View", ubo.view);
    material->SetAllUniformData("type.ConstantBuffer.UBO", "u_Projection", ubo.proj);
}

TexCube::~TexCube()
{
    cleanup();
}

void TexCube::Update(float DeltaTime)
{
    Component::Update(DeltaTime);
        
    MAT4(m);

    // 平移
    m = glm::translate(m, GetPos());
    
    // 旋转
    glm::quat quat = VEC3_ZERO;
    
    quat *= glm::angleAxis((float)glfwGetTime(), glm::vec3{1, 0, 0});

    glm::mat4 rotationMatrix = glm::mat4_cast(quat);
    m *= rotationMatrix;
    
    material->SetCurrentUniformData("type.ConstantBuffer.UBO", "model", m);
}

void TexCube::Draw(const RenderInfo& RenderInfo)
{
    Component::Draw(RenderInfo);

    material->Draw(RenderInfo);

    buffer.CmdBind(RenderInfo.CommmandBuffer);
    
    vkCmdDraw(RenderInfo.CommmandBuffer, sizeof(BoxVertices) / 8, 1, 0, 0);
}

MaterialRenderPipelineInfoTexCube::MaterialRenderPipelineInfoTexCube(const string& shaderName, ShaderCodeType codeType)
    :MaterialRenderPipelineInfo(shaderName, codeType)
{
    
}

VkDescriptorSetLayout MaterialRenderPipelineInfoTexCube::MakeVkDescriptorSetLayout() const
{
    if(descriptorSetLayout)
    {
        return descriptorSetLayout;
    }

    std::vector<VkDescriptorSetLayoutBinding> out;
    VertShader->FillDescriptorSetLayoutBinding(out);
    FragShader->FillDescriptorSetLayoutBinding(out);
 
    VkDescriptorSetLayoutCreateInfo layoutInfo = {};
    layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    layoutInfo.bindingCount = (uint32_t)out.size();
    layoutInfo.pBindings = out.data();
 
    if (vkCreateDescriptorSetLayout(GDevice, &layoutInfo, nullptr, &descriptorSetLayout) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create descriptor set layout!");
    }
    
    return descriptorSetLayout;
}


void DrawTexCube::Init()
{
    Level::Init();

    {
        auto obj = NewObject();
        obj->Attach(NewSPtr<TexCube>());
    }
}
