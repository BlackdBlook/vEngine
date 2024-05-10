#include "DrawSkybox.h"

#include "Component/Common/BasicMove/BasicCameraMove.h"
#include "Component/Common/EscExit/PressEscExit.h"
#include "Engine/vEngine.h"
#include "Engine/Core/TextureCube.h"
#include "Engine/Core/Component/Component.h"
#include "Engine/Core/FrameInfo/RenderInfo.h"
#include "Engine/Core/Object/Object.h"
#include "Engine/Runtime/Component/CubeComponent.h"
#include "Meshs/Box/BoxVertices.h"

namespace
{
    class SkyboxMaterialRenderPipelineInfo: public MaterialRenderPipelineInfo
    {
    public:
        SkyboxMaterialRenderPipelineInfo(const string& shader);
        virtual VkPipelineVertexInputStateCreateInfo* PipelineVertexInputStateCreateInfo() const override;
    };
    
    struct SkyBoxMeshVertex
    {
        glm::vec3 Pos;

        static VkVertexInputBindingDescription getBindingDescription();

        static std::array<VkVertexInputAttributeDescription, 1> getAttributeDescriptions();
    };

    SkyboxMaterialRenderPipelineInfo::SkyboxMaterialRenderPipelineInfo(const string& shader) :
        MaterialRenderPipelineInfo(shader)
    {
;
    }

    VkPipelineVertexInputStateCreateInfo* SkyboxMaterialRenderPipelineInfo::PipelineVertexInputStateCreateInfo() const
    {
        static VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
        static auto bindingDescription = SkyBoxMeshVertex::getBindingDescription();
        static auto attributeDescriptions =
            SkyBoxMeshVertex::getAttributeDescriptions();
        vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
        vertexInputInfo.vertexBindingDescriptionCount = 1;
        vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
        vertexInputInfo.pVertexBindingDescriptions = &bindingDescription;
        vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();

        return &vertexInputInfo;
    }

    VkVertexInputBindingDescription SkyBoxMeshVertex::getBindingDescription()
    {
        VkVertexInputBindingDescription bindingDescription{};

        bindingDescription.binding = 0;
        bindingDescription.stride = sizeof(SkyBoxMeshVertex);
        bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

        return bindingDescription;
    }

    std::array<VkVertexInputAttributeDescription, 1> SkyBoxMeshVertex::getAttributeDescriptions()
    {
        std::array<VkVertexInputAttributeDescription, 1> attributeDescriptions{};
        attributeDescriptions[0].binding = 0;
        attributeDescriptions[0].location = 0;
        attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[0].offset = offsetof(SkyBoxMeshVertex, Pos);
        
        return attributeDescriptions;
    }

    class SkyBoxMeshVertexBuffer
    {
        VkBuffer Buffer;

        VkDeviceMemory vertexBufferMemory;
    
    public:
        SkyBoxMeshVertexBuffer(size_t Size, const void* Data);
        ~SkyBoxMeshVertexBuffer();
    
        void CmdBind(VkCommandBuffer CommandBuffer);

    };

    SkyBoxMeshVertexBuffer::SkyBoxMeshVertexBuffer(size_t Size, const void* Data)
    {
        if(Size == 0)
        {
            return;
        }

        VkBufferCreateInfo bufferInfo{};
        bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        bufferInfo.size = Size;
        bufferInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
        bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

        if (vkCreateBuffer(GDevice, &bufferInfo, nullptr, &Buffer) != VK_SUCCESS) {
            throw std::runtime_error("failed to create vertex buffer!");
        }

        VkMemoryRequirements memRequirements;
        vkGetBufferMemoryRequirements(GDevice, Buffer, &memRequirements);

        VkMemoryAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        allocInfo.allocationSize = memRequirements.size;
        allocInfo.memoryTypeIndex = VkHelperInstance->FindMemoryType(memRequirements.memoryTypeBits,
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

        if (vkAllocateMemory(GDevice, &allocInfo, nullptr, &vertexBufferMemory) != VK_SUCCESS) {
            throw std::runtime_error("failed to allocate vertex buffer memory!");
        }

        vkBindBufferMemory(GDevice, Buffer, vertexBufferMemory, 0);
        
        void* data;
        vkMapMemory(GDevice, vertexBufferMemory, 0, bufferInfo.size, 0, &data);
        memcpy(data, Data, Size);
        vkUnmapMemory(GDevice, vertexBufferMemory);
    }

    SkyBoxMeshVertexBuffer::~SkyBoxMeshVertexBuffer()
    {
        vkDestroyBuffer(GDevice, Buffer, nullptr);
        vkFreeMemory(GDevice, vertexBufferMemory, nullptr);
    }

    void SkyBoxMeshVertexBuffer::CmdBind(VkCommandBuffer CommandBuffer)
    {
        assert(CommandBuffer != nullptr);
        VkBuffer vertexBuffers[] = {Buffer};
        VkDeviceSize offsets[] = {0};
        vkCmdBindVertexBuffers(CommandBuffer, 0, 1, vertexBuffers, offsets);
    }


    
    
    class Skybox : public Component
    {
        SPtr<Material> material;

        SkyBoxMeshVertexBuffer buffer;
    public:
        Skybox(const string& shader);

        ~Skybox() override;

        virtual void Update(float DeltaTime) override;

        virtual void Draw(const RenderInfo& RenderInfo) override; 
    };




    Skybox::Skybox(const string& shader):
        buffer(sizeof(SkyBoxVertices), SkyBoxVertices)
    {
        SPtr<MaterialRenderPipelineInfo> info = NewSPtr<SkyboxMaterialRenderPipelineInfo>(shader);
        material = NewSPtr<Material>(info);
    }

    Skybox::~Skybox()
    {
    }

    void Skybox::Update(float DeltaTime)
    {
        Component::Update(DeltaTime);
    
    }

    void Skybox::Draw(const RenderInfo& RenderInfo)
    {
        Component::Draw(RenderInfo);

        auto model = Parent->GetModelMat();

        material->SetCurrentUniformData("model.model", model);
    
        material->Draw(RenderInfo);

        buffer.CmdBind(RenderInfo.CommmandBuffer);
    
        vkCmdDraw(RenderInfo.CommmandBuffer,
            sizeof(BoxVertices) / 8, 1, 0, 0);
    
    }








}


void DrawSkybox::Init()
{
    Level::Init();

    {
        auto obj = NewObject();
        obj->CreateAttach<BasicCameraMove>();
        obj->CreateAttach<PressEscExit>();
    }
    
    {
        auto obj = NewObject();

        obj->CreateAttach<Skybox>("CommonSkyBox");
    }
}

LevelRegister(DrawSkybox);
