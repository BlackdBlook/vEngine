#include "Material.h"

#include "MaterialRenderPipeline.h"
#include "Engine/vEngine.h"
#include "Engine/Core/FrameInfo/RenderInfo.h"
#include "Engine/Core/ShaderModule/Shader.h"
#include "Engine/Toolkit/FileToolKit/FileToolKit.h"



Material::Material()
{
    
}

Material::Material(const string& shaderName) : info{shaderName}
{
    auto setLayout = info.MakeVkDescriptorSetLayout();
    
    descriptor.createDescriptorSets(setLayout);

    std::vector<UniformBuffer> buffers = info.MakeUniformBuffers();

    descriptor.BindMemoryBuffer(std::move(buffers));
    
    pipeline.Init(&info);
}

Material::Material(const string& VertShaderName, const string& FragShaderName) : info{VertShaderName, FragShaderName}
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

void Material::SetUniformData(uint32 index, uint8* Src, size_t Size, uint32 Offset)
{
    descriptor.buffers[index].UpdateBuffer(Src, Size, Offset);
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
