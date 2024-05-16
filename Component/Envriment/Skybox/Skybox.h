#pragma once
#include "Engine/Core/Component/Component.h"
#include "Engine/Core/Component/SceneComponent/SceneComponent.h"
#include "Engine/Core/MemoryBuffer/MeshVertexBuffer.h"

class Material;

class SkyBoxMeshVertexBuffer
{
    VkBuffer Buffer;

    VkDeviceMemory vertexBufferMemory;
    
public:
    SkyBoxMeshVertexBuffer(size_t Size, const void* Data);
    ~SkyBoxMeshVertexBuffer();
    
    void CmdBind(VkCommandBuffer CommandBuffer);

};

struct SkyBoxMeshVertex
{
    glm::vec3 Pos;

    static VkVertexInputBindingDescription getBindingDescription();

    static std::array<VkVertexInputAttributeDescription, 1> getAttributeDescriptions();
};

class Skybox : public SceneComponent
{
    SPtr<Material> material;

    SkyBoxMeshVertexBuffer buffer;
public:
    Skybox(const string& shader);

    ~Skybox() override;

    virtual void Update(float DeltaTime) override;

    virtual void Draw(const RenderInfo& RenderInfo) override; 
};
