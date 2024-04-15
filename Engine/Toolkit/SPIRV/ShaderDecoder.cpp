#include "ShaderDecoder.h"
#include "ThirdParty/SPIRV-Cross/spirv_cross.hpp"
void ShaderDecoder::GetShaderResourceInfos(
    const std::vector<uint8>& ShaderBinary, ShaderResourceInfos& out)
{
    spirv_cross::Compiler tempShader(
        (const uint32_t*)ShaderBinary.data(), ShaderBinary.size() / sizeof(uint32_t));

    // The SPIR-V is now parsed, and we can perform reflection on it.
    spirv_cross::ShaderResources resources = tempShader.get_shader_resources();

    // Get all sampled images in the shader.
    for (auto& resource : resources.uniform_buffers)
    {
        const uint32 set = tempShader.get_decoration(resource.id, spv::DecorationDescriptorSet);
        const uint32 binding = tempShader.get_decoration(resource.id, spv::DecorationBinding);
        const uint32 size = tempShader.get_decoration(resource.id, spv::DecorationUniform);

        out.UniformBuffer.push_back(ResourceInfo{std::move(resource.name), set, binding});
    }
 
    
}
