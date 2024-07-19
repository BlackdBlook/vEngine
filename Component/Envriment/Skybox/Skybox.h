#pragma once
#include "Engine/Core/Component/Component.h"
#include "Engine/Core/Component/SceneComponent/SceneComponent.h"
#include "Engine/Core/MemoryBuffer/MeshVertexBuffer.h"

class Material;

class SkyBoxMeshVertexBuffer : public VertexBuffer
{
public:
    SkyBoxMeshVertexBuffer(size_t Size, const void* Data);
    ~SkyBoxMeshVertexBuffer() override;
    
    void CmdBind(VkCommandBuffer CommandBuffer) override;

};

struct SkyBoxMeshVertex
{
    glm::vec3 Pos;

    static VkVertexInputBindingDescription getBindingDescription();

    static std::array<VkVertexInputAttributeDescription, 1> getAttributeDescriptions();

    static size_t GetVertexStep();
};

class Skybox : public SceneComponent
{
    SPtr<Material> material;

    SkyBoxMeshVertexBuffer buffer;
public:
    Skybox(const string& shader);

    ~Skybox() override;

    virtual SceneComponentRenderInfo* GenRenderInfo() override;

    virtual void Draw(FrameRenderInfo& RenderInfo) override; 
};
