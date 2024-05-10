#include "Material.h"

#include "MaterialRenderPipeline.h"
#include "Engine/vEngine.h"
#include "Engine/Core/FrameInfo/RenderInfo.h"
#include "Engine/Core/ShaderModule/Shader.h"
#include "Engine/Core/Texture2D/Texture2D.h"
#include "Engine/Toolkit/FileToolKit/FileToolKit.h"
#include "LogPrinter/Log.h"


Material::Material()
{
    
}

void Material::Init()
{
    descriptor = NewSPtr<DescriptorHelper>(info.get());

    pipeline.Init(info.get());
}

Material::Material(const string& shaderName, ShaderCodeType codeType)
: info(NewSPtr<MaterialRenderPipelineInfo>(shaderName, codeType))
{
    Init();
}

Material::Material(const string& VertShaderName, const string& FragShaderName, ShaderCodeType codeType)
: info(NewSPtr<MaterialRenderPipelineInfo>(VertShaderName, FragShaderName, codeType))
{
    Init();
}

Material::Material(SPtr<MaterialRenderPipelineInfo> Info)
    :info(std::move(Info))
{
    Init();
}

Material::~Material()
{
    
}

void Material::SetTexture(const string& TargetName, const string& NewTextureName)
{
    auto target = descriptor->Textures.find(TargetName);
    if(target == descriptor->Textures.end())
    {
        ERR("Texture not found");
        return;
    }
    target->second->SetTexture(NewTextureName);
    descriptor->BindInputBuffer();
}

void Material::SetCurrentUniformData(const Container::Name& MemberName, uint8* Src, size_t Size, size_t Offset)
{
    auto blockName = info->UniformBufferBlocks->UniformMemberInBlocks.find(MemberName);
    if(blockName == info->UniformBufferBlocks->UniformMemberInBlocks.end())
    {
        return;
    }

    auto buffer = descriptor->buffers.find(blockName->second);
    if(buffer == descriptor->buffers.end())
    {
        return;
    }

    buffer->second->UpdateCurrentBuffer(Src, Size, Offset);
    
}

void Material::SetAllUniformData(const Container::Name& MemberName, uint8* Src, size_t Size, size_t Offset)
{
    auto blockName = info->UniformBufferBlocks->UniformMemberInBlocks.find(MemberName);
    if(blockName == info->UniformBufferBlocks->UniformMemberInBlocks.end())
    {
        return;
    }

    auto buffer = descriptor->buffers.find(blockName->second);
    if(buffer == descriptor->buffers.end())
    {
        return;
    }

    buffer->second->UpdateAllBuffer(Src, Size, Offset);
}

void Material::SetCurrentUniformData(const Container::Name& MemberName, uint8* Src, size_t Size)
{
    auto target = info->UniformBufferBlocks->UniformMembers.find(MemberName);
    if(target != info->UniformBufferBlocks->UniformMembers.end())
    {
        SetCurrentUniformData(MemberName, Src, Size, target->second.Offset);
    }
}

void Material::SetAllUniformData(const Container::Name& MemberName, uint8* Src, size_t Size)
{
    auto target = info->UniformBufferBlocks->UniformMembers.find(MemberName);
    if(target != info->UniformBufferBlocks->UniformMembers.end())
    {
        SetAllUniformData(MemberName, Src, Size, target->second.Offset);
    }
}

void Material::Draw(const RenderInfo& RenderInfo)
{
    pipeline.CmdBind(RenderInfo.CommmandBuffer);

    vkCmdBindDescriptorSets(
        RenderInfo.CommmandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS,
        info->PipelineLayout(), 0, 1,
        descriptor->GetDescriptorSetsByCurrentFrameIndex(),
        0, nullptr);
}
