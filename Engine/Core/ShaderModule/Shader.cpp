#include "Shader.h"

#include <filesystem>

#include "Engine/vEngine.h"
#include "Engine/SubSystem/AssetSystem/AssetSystem.h"
#include "Engine/Toolkit/FileToolKit/FileToolKit.h"
#include "LogPrinter/Log.h"
#include "ThirdParty/SPIRV-Cross/spirv_cross.hpp"

VkShaderModule createShaderModule(const std::vector<uint8>& code) {
    VkShaderModuleCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = code.size();
    createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

    VkShaderModule shaderModule;
    if (vkCreateShaderModule(GDevice, &createInfo, nullptr, &shaderModule) != VK_SUCCESS) {
        throw std::runtime_error("failed to create shader module!");
    }
    return shaderModule;
}

ShaderUniformBufferBlocks ShaderDecoder::decodeUniformBuffer(std::vector<uint8>* binaryShader)
{
    if(binaryShader->size() == 0)
    {
        throw new std::runtime_error("binaryShader is empty");
    }
    ShaderUniformBufferBlocks ans;
    std::vector<uint32_t>& temp = reinterpret_cast<std::vector<uint32_t>&>(*binaryShader);
    spirv_cross::Compiler glsl(temp);
    
    // 获取反射数据
    spirv_cross::ShaderResources resources = glsl.get_shader_resources();

    // 遍历所有的uniform变量
    for (auto& resource : resources.uniform_buffers)
    {
        ShaderUniformBufferBlock block;
        const spirv_cross::SPIRType& type = glsl.get_type(resource.base_type_id);
        
        // 遍历uniform变量的所有成员
        for (size_t i = 0; i < type.member_types.size(); i++)
        {
            const spirv_cross::SPIRType& memberType = glsl.get_type(type.member_types[i]);
            
            ShaderUniformMember member;
            member.Name = glsl.get_member_name(type.self, static_cast<uint32_t>(i));
            member.Size = glsl.get_declared_struct_member_size(type, static_cast<uint32_t>(i));
            member.Offset = glsl.type_struct_member_offset(type, static_cast<uint32_t>(i));
            
            block.Members.insert({member.Name, std::move(member)});
        }


        uint32_t set = glsl.get_decoration(resource.id, spv::DecorationDescriptorSet);
        uint32_t binding = glsl.get_decoration(resource.id, spv::DecorationBinding);
        
        block.Size = glsl.get_declared_struct_size(type);
        block.Bind = binding;
        block.Set = set;
        
        ans.UniformBlocks.insert({resource.name, block});
    }

    return ans;
}

std::string ShaderUniformMember::Log()
{
    return "Name:" + Name + " ,Size:" + std::to_string(Size) + " ,Offset:" + std::to_string(Offset);
}

std::string ShaderUniformBufferBlock::Log()
{
    string ans = "Uniform Block " + Name + ",Size:" + std::to_string(Size) +
        ",Set:" + std::to_string(Set) + ",Bind:" + std::to_string(Bind) + '\n';
    
    for(auto& m : Members)
    {
        ans += '\t' + m.second.Log() + '\n';
    }

    return ans;
}

void ShaderUniformBufferBlocks::Log()
{
#ifdef _DEBUG
    string log;
    for(auto& b : UniformBlocks)
    {
        log += b.second.Log() + '\n';
    }
    LOG(log);
#endif
}

VkShaderStageFlagBits vShader::GetVkShaderStageFlagBits()
{
    switch (type)
    {
    case ShaderType::Vertex:
        return VK_SHADER_STAGE_VERTEX_BIT;
    case ShaderType::Fragment:
        return VK_SHADER_STAGE_FRAGMENT_BIT;
    }
    return VK_SHADER_STAGE_ALL;
}

vShader::vShader(const char* Name, ShaderType type, ShaderCodeType codeType): type(type)
{
    string fileName = Name;
    
    if(codeType == ShaderCodeType::GLSL)
    {
        switch (type) {
        case ShaderType::Vertex:
            fileName += ".vert.spv";
            break;
        case ShaderType::Fragment:
            fileName += ".frag.spv";
            break;
        }
        EntryPointFunctionName = "main";
    }
    else
    {
        switch (type) {
        case ShaderType::Vertex:
            fileName += ".vs.spv";
            EntryPointFunctionName = "VS";
            break;
        case ShaderType::Fragment:
            fileName += ".ps.spv";
            EntryPointFunctionName = "PS";
            break;
        }
    }
    
    auto Path = AssetSystem::GetInstance()->GetFilePathByName(fileName);

    auto data = FileToolKit::ReadFileAsBinary(Path);

    ShaderUniformBufferBlocks = ShaderDecoder::decodeUniformBuffer(&data);

    ShaderUniformBufferBlocks.Log();

    vkshadermodule = createShaderModule(data);
    
}


vShader::~vShader()
{
    vkDestroyShaderModule(GDevice, vkshadermodule, nullptr);
}

VkPipelineShaderStageCreateInfo vShader::GetStageInfo()
{
    VkPipelineShaderStageCreateInfo vertShaderStageInfo{};
    vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    vertShaderStageInfo.stage = GetVkShaderStageFlagBits();
    vertShaderStageInfo.module = vkshadermodule;
    vertShaderStageInfo.pName = EntryPointFunctionName.c_str();
    return vertShaderStageInfo;
}

void vShader::FillDescriptorSetLayoutBinding(std::vector<VkDescriptorSetLayoutBinding>& out)
{
    for(auto& block :
        ShaderUniformBufferBlocks.UniformBlocks)
    {
        VkDescriptorSetLayoutBinding uboLayoutBinding = {};

        // 绑定点
        uboLayoutBinding.binding = block.second.Bind;
        // 类型
        uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        // 数组数量
        uboLayoutBinding.descriptorCount = 1;
        // 作用阶段
        uboLayoutBinding.stageFlags = GetVkShaderStageFlagBits();
        // 纹理采样器
        uboLayoutBinding.pImmutableSamplers = nullptr;
        
        out.emplace_back(uboLayoutBinding);
    }
}
