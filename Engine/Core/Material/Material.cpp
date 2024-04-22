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

    std::vector<UniformBuffer> buffers = info->MakeUniformBuffers();

    descriptor.buffers = std::move(buffers);

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

void Material::SetCurrentUniformData(uint32 index, uint8* Src, size_t Size, size_t Offset)
{
    descriptor.buffers[index].UpdateCurrentBuffer(Src, Size, Offset);
}

void Material::SetAllUniformData(uint32 index, uint8* Src, size_t Size, size_t Offset)
{
    descriptor.buffers[index].UpdateAllBuffer(Src, Size, Offset);
}

void Material::SetCurrentUniformData(const string& BlockName, uint8* Src, size_t Size, size_t Offset)
{
    for (auto& block : descriptor.buffers)
    {
        if(block.BlockName != BlockName)
        {
            continue;
        }

        block.UpdateCurrentBuffer(Src, Size, Offset);
    }
}

void Material::SetAllUniformData(const string& BlockName, uint8* Src, size_t Size, size_t Offset)
{
    for (auto& block : descriptor.buffers)
    {
        if(block.BlockName != BlockName)
        {
            continue;
        }

        block.UpdateAllBuffer(Src, Size, Offset);
    }
}

void Material::SetCurrentUniformData(const string& BlockName, const string& MemberName, uint8* Src, size_t Size)
{
    SetCurrentUniformData(BlockName, Src, Size,
        info->VertShader->ShaderUniformBufferBlocks.UniformBlocks[BlockName].Members[MemberName].Offset);
}

void Material::SetAllUniformData(const string& BlockName, const string& MemberName, uint8* Src, size_t Size)
{
    SetAllUniformData(BlockName, Src, Size,
        info->VertShader->ShaderUniformBufferBlocks.UniformBlocks[BlockName].Members[MemberName].Offset);
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
