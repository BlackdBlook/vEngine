#pragma once
#include <array>

#include "MaterialRenderPipeline.h"
#include "Engine/TypeDef.h"
#include "Engine/Core/MemoryBuffer/MeshVertexBuffer.h"
#include "Engine/Core/ShaderModule/DescriptorHelper.h"
#include "Engine/Core/ShaderModule/Shader.h"
#include "Engine/Core/UniformBuffer/UniformBuffer.h"

class MaterialRenderPipeline;
class MaterialRenderPipelineInfo;
class RenderInfo;

struct Vertex
{
    glm::vec3 Pos;
    glm::vec3 Normal;
    glm::vec2 TexCoords;

    static VkVertexInputBindingDescription getBindingDescription()
    {
        VkVertexInputBindingDescription bindingDescription{};

        bindingDescription.binding = 0;
        bindingDescription.stride = sizeof(Vertex);
        bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

        return bindingDescription;
    }

    static std::array<VkVertexInputAttributeDescription, 3> getAttributeDescriptions()
    {
        std::array<VkVertexInputAttributeDescription, 3> attributeDescriptions{};
        attributeDescriptions[0].binding = 0;
        attributeDescriptions[0].location = 0;
        attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[0].offset = offsetof(Vertex, Pos);

        attributeDescriptions[1].binding = 0;
        attributeDescriptions[1].location = 1;
        attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[1].offset = offsetof(Vertex, Normal);

        attributeDescriptions[2].binding = 0;
        attributeDescriptions[2].location = 2;
        attributeDescriptions[2].format = VK_FORMAT_R32G32_SFLOAT;
        attributeDescriptions[2].offset = offsetof(Vertex, TexCoords);
        return attributeDescriptions;
    }
};



class Material
{
public:
    MaterialRenderPipeline pipeline;

    DescriptorHelper descriptor;

    SPtr<MaterialRenderPipelineInfo> info;
    
    Material();

    Material(const string& shaderName, ShaderCodeType codeType = ShaderCodeType::HLSL);
    
    Material(const string& VertShaderName, const string& FragShaderName, ShaderCodeType codeType = ShaderCodeType::HLSL);

    Material(SPtr<MaterialRenderPipelineInfo> Info);

    ~Material();

    void SetCurrentUniformData(uint32 index, uint8* Src, size_t Size, size_t Offset = 0);
    void SetAllUniformData(uint32 index, uint8* Src, size_t Size, size_t Offset = 0);
    
    void SetCurrentUniformData(const string& BlockName, uint8* Src, size_t Size, size_t Offset = 0);
    void SetAllUniformData(const string& BlockName, uint8* Src, size_t Size, size_t Offset = 0);
    
    void SetCurrentUniformData(const string& BlockName, const string& MemberName, uint8* Src, size_t Size);
    void SetAllUniformData(const string& BlockName, const string& MemberName, uint8* Src, size_t Size);

    template<typename T>
    void SetCurrentUniformData(const string& BlockName, const string& MemberName, const T& Src);
    template<typename T>
    void SetAllUniformData(const string& BlockName, const string& MemberName, const T& Src);

    void Draw(const RenderInfo& RenderInfo);
};

template <typename T>
void Material::SetCurrentUniformData(const string& BlockName, const string& MemberName, const T& Src)
{
    SetCurrentUniformData(BlockName, MemberName, (uint8*)&Src, sizeof(T));
}

template <typename T>
void Material::SetAllUniformData(const string& BlockName, const string& MemberName, const T& Src)
{
    SetAllUniformData(BlockName, MemberName, (uint8*)&Src, sizeof(T));
}