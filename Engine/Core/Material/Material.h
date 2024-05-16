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


class Material
{
    Material();

    void Init();
    
public:
    MaterialRenderPipeline pipeline;

    SPtr<DescriptorHelper> descriptor;

    SPtr<MaterialRenderPipelineInfo> info;

    SPtr<ShaderUniformBufferBlocks> ShaderUniformBufferBlocks;

    Material(const string& shaderName, ShaderCodeType codeType = ShaderCodeType::HLSL);
    
    Material(const string& VertShaderName, const string& FragShaderName, ShaderCodeType codeType = ShaderCodeType::HLSL);

    Material(SPtr<MaterialRenderPipelineInfo> Info);

    ~Material();

    void SetTexture(const string& TargetName, const string& NewTextureName);
    
    void SetCurrentUniformData(const Container::Name& MemberName, uint8* Src, size_t Size, size_t Offset);
    void SetAllUniformData(const Container::Name& MemberName, uint8* Src, size_t Size, size_t Offset);
    
    void SetCurrentUniformData(const Container::Name& MemberName, uint8* Src, size_t Size);
    void SetAllUniformData(const Container::Name& MemberName, uint8* Src, size_t Size);

    template<typename T>
    void SetCurrentUniformData(const Container::Name& MemberName, const T& Src);
    template<typename T>
    void SetAllUniformData(const Container::Name& MemberName, const T& Src);

    void Draw(const RenderInfo& RenderInfo);
};

template <typename T>
void Material::SetCurrentUniformData(const Container::Name& MemberName, const T& Src)
{
    SetCurrentUniformData(MemberName, (uint8*)&Src, sizeof(T));
}

template <typename T>
void Material::SetAllUniformData(const Container::Name& MemberName, const T& Src)
{
    SetAllUniformData(MemberName, (uint8*)&Src, sizeof(T));
}