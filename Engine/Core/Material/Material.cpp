#include "Material.h"

#include "MaterialRenderPipeline.h"
#include "Engine/vEngine.h"
#include "Engine/Core/FrameInfo/RenderInfo.h"
#include "Engine/Core/ShaderModule/Shader.h"
#include "Engine/Toolkit/FileToolKit/FileToolKit.h"



Material::Material()
{
    
}

Material::Material(const string& shaderName, ShaderCodeType codeType) : info{shaderName, codeType}
{
    auto setLayout = info.MakeVkDescriptorSetLayout();
    
    descriptor.createDescriptorSets(setLayout);

    std::vector<UniformBuffer> buffers = info.MakeUniformBuffers();

    descriptor.BindMemoryBuffer(std::move(buffers));
    
    pipeline.Init(&info);
}

Material::Material(const string& VertShaderName, const string& FragShaderName, ShaderCodeType codeType) : info{VertShaderName, FragShaderName, codeType}
{
    auto setLayout = info.MakeVkDescriptorSetLayout();
    
    descriptor.createDescriptorSets(setLayout);

    std::vector<UniformBuffer> buffers = info.MakeUniformBuffers();

    descriptor.BindMemoryBuffer(std::move(buffers));
    
    pipeline.Init(&info);
}

Material::Material(MaterialRenderPipelineInfo& info)
{
    pipeline.Init(&info);
}

Material::~Material()
{
    
}

void Material::SetUniformData(uint32 index, uint8* Src, size_t Size, size_t Offset)
{
    descriptor.buffers[index].UpdateBuffer(Src, Size, Offset);
}

void Material::SetUniformData(string BlockName, uint8* Src, size_t Size, size_t Offset)
{
    for (auto& block : descriptor.buffers)
    {
        if(block.BlockName != BlockName)
        {
            continue;
        }

        block.UpdateBuffer(Src, Size, Offset);
    }
}

void Material::SetUniformData(string BlockName, string MemberName, uint8* Src, size_t Size)
{
    SetUniformData(BlockName, Src, Size,
        info.VertShader->ShaderUniformBufferBlocks.UniformBlocks[BlockName].Members[MemberName].Offset);
}

void Material::Draw(const RenderInfo& RenderInfo)
{
    pipeline.CmdBind(RenderInfo.CommmandBuffer);

    vkCmdBindDescriptorSets(
        RenderInfo.CommmandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS,
        info.PipelineLayout(), 0, 1,
        descriptor.GetDescriptorSetsByCurrentFrameIndex(),
        0, nullptr);
}
