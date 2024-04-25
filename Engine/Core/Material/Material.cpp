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
    auto setLayout = info->MakeVkDescriptorSetLayout();
    
    info->MakeInputTextures(descriptor.Texture2Ds);
    
    descriptor.createDescriptorSets(setLayout);

    descriptor.buffers = info->MakeUniformBuffers();

    descriptor.BindInputBuffer();
    
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
    auto target = descriptor.Texture2Ds.find(TargetName);
    if(target == descriptor.Texture2Ds.end())
    {
        ERR("Texture not found");
        return;
    }
    target->second->SetTexture(NewTextureName);
    descriptor.BindInputBuffer();
}

void Material::SetCurrentUniformData(const Container::Name& MemberName, uint8* Src, size_t Size, size_t Offset)
{
    bool set = false;
    do
    {
        auto blockName = info->VertShader->UniformBufferBlocks.UniformMemberInBlocks.find(MemberName);
        if(blockName == info->VertShader->UniformBufferBlocks.UniformMemberInBlocks.end())
        {
            break;
        }

        auto buffer = descriptor.buffers.find(blockName->second);
        if(buffer == descriptor.buffers.end())
        {
            break;
        }

        buffer->second->UpdateCurrentBuffer(Src, Size, Offset);
        set = true;
    }while(false);
    do
    {
        auto blockName = info->FragShader->UniformBufferBlocks.UniformMemberInBlocks.find(MemberName);
        if(blockName == info->FragShader->UniformBufferBlocks.UniformMemberInBlocks.end())
        {
            break;
        }

        auto buffer = descriptor.buffers.find(blockName->second);
        if(buffer == descriptor.buffers.end())
        {
            break;
        }

        buffer->second->UpdateCurrentBuffer(Src, Size, Offset);
        set = true;
    }while(false);

    if(set == false)
    {
        ERR(MemberName, "Not Find");
    }
}

void Material::SetAllUniformData(const Container::Name& MemberName, uint8* Src, size_t Size, size_t Offset)
{
    bool set = false;
    do
    {
        auto blockName = info->VertShader->UniformBufferBlocks.UniformMemberInBlocks.find(MemberName);
        if(blockName == info->VertShader->UniformBufferBlocks.UniformMemberInBlocks.end())
        {
            break;
        }

        auto buffer = descriptor.buffers.find(blockName->second);
        if(buffer == descriptor.buffers.end())
        {
            break;
        }

        buffer->second->UpdateAllBuffer(Src, Size, Offset);
        set = true;
    }while(false);
    do
    {
        auto blockName = info->FragShader->UniformBufferBlocks.UniformMemberInBlocks.find(MemberName);
        if(blockName == info->FragShader->UniformBufferBlocks.UniformMemberInBlocks.end())
        {
            break;
        }

        auto buffer = descriptor.buffers.find(blockName->second);
        if(buffer == descriptor.buffers.end())
        {
            break;
        }

        buffer->second->UpdateAllBuffer(Src, Size, Offset);
        set = true;
    }while(false);

    if(set == false)
    {
        ERR(MemberName, "Not Find");
    }
}

void Material::SetCurrentUniformData(const Container::Name& MemberName, uint8* Src, size_t Size)
{
    {
        auto target = info->VertShader->UniformBufferBlocks.UniformMembers.find(MemberName);
        if(target != info->VertShader->UniformBufferBlocks.UniformMembers.end())
        {
            SetCurrentUniformData(MemberName, Src, Size, target->second.Offset);
        }
    }

    {
        auto target = info->FragShader->UniformBufferBlocks.UniformMembers.find(MemberName);
        if(target != info->FragShader->UniformBufferBlocks.UniformMembers.end())
        {
            SetCurrentUniformData(MemberName, Src, Size, target->second.Offset);
        }
    }
    
}

void Material::SetAllUniformData(const Container::Name& MemberName, uint8* Src, size_t Size)
{
    {
        auto target = info->VertShader->UniformBufferBlocks.UniformMembers.find(MemberName);
        if(target != info->VertShader->UniformBufferBlocks.UniformMembers.end())
        {
            SetAllUniformData(MemberName, Src, Size, target->second.Offset);
        }
    }

    {
        auto target = info->FragShader->UniformBufferBlocks.UniformMembers.find(MemberName);
        if(target != info->FragShader->UniformBufferBlocks.UniformMembers.end())
        {
            SetAllUniformData(MemberName, Src, Size, target->second.Offset);
        }
    }
}

void Material::Draw(const RenderInfo& RenderInfo)
{
    pipeline.CmdBind(RenderInfo.CommmandBuffer);

    vkCmdBindDescriptorSets(
        RenderInfo.CommmandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS,
        info->PipelineLayout(), 0, 1,
        descriptor.GetDescriptorSetsByCurrentFrameIndex(),
        0, nullptr);
}
