#pragma once

#include "MaterialRenderPipeline.h"
#include "Engine/TypeDef.h"
#include "Engine/Core/ShaderModule/DescriptorHelper.h"
#include "Engine/Core/ShaderModule/Shader.h"
#include "Engine/Core/UniformBuffer/UniformBuffer.h"

class MaterialRenderPipelineInfo;
class FrameRenderInfo;

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
 
    void SetTexture(const string& TargetName,
        const string& NewTextureName, bool ClearOld = true);
    void SetTexture(const string& TargetName,
        VkImageView ImageView, bool ClearOld = true);
    void SetTextureAtFream(const string& TargetName,
        VkImageView ImageView, uint32 FreamIndex, bool ClearOld = true);
    
    void SetCurrentUniformData(const Container::Name& MemberName, uint8* Src, size_t Size, size_t Offset);
    void SetAllUniformData(const Container::Name& MemberName, uint8* Src, size_t Size, size_t Offset);
    
    void SetCurrentUniformData(const Container::Name& MemberName, uint8* Src, size_t Size);
    void SetAllUniformData(const Container::Name& MemberName, uint8* Src, size_t Size);

    template<typename T>
    void SetCurrentUniformData(const Container::Name& MemberName, const T& Src);
    template<typename T>
    void SetAllUniformData(const Container::Name& MemberName, const T& Src);

    void Draw(const FrameRenderInfo& RenderInfo);
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