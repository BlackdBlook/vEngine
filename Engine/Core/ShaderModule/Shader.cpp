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

VkDescriptorType ShaderTextureInput::GetDescriptorType()
{
    switch (type) {
    case spirv_cross::SPIRType::Image:
        return VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE;
        break;
    case spirv_cross::SPIRType::SampledImage:
        return VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        break;
    case spirv_cross::SPIRType::Sampler:
        return VK_DESCRIPTOR_TYPE_SAMPLER;
        break;
    default:
        throw std::runtime_error("type error");
    }
}

std::string ShaderTextureInput::Log()
{
    string ans = "Texture Input " + name +
        ",Set:" + std::to_string(set) +
        ",Bind:" + std::to_string(bind) +
        ",type:" + std::to_string(type) +
        '\n';
    return ans;
}

void ShaderTextureInputs::Log()
{
#ifdef _DEBUG
    string log;
    for(auto& b : Members)
    {
        log += b.second.Log();
        log += '\n';
    }
    if(!log.empty())
    {
        LOG(log);
    }
#endif
}

ShaderUniformBufferBlocks ShaderDecoder::decodeUniformBuffer(std::vector<uint8>* binaryShader)
{
    if(binaryShader->empty())
    {
        throw std::runtime_error("binaryShader is empty");
    }
    ShaderUniformBufferBlocks ans;
    std::vector<uint32_t>& temp = reinterpret_cast<std::vector<uint32_t>&>(*binaryShader);
    spirv_cross::Compiler compiler(temp);
    
    // 获取反射数据
    spirv_cross::ShaderResources resources = compiler.get_shader_resources();

    // 遍历所有的uniform变量
    for (auto& resource : resources.uniform_buffers)
    {
        ShaderUniformBufferBlock block;
        const spirv_cross::SPIRType& type = compiler.get_type(resource.base_type_id);
        
        // 遍历uniform变量的所有成员
        for (size_t i = 0; i < type.member_types.size(); i++)
        {
            const spirv_cross::SPIRType& memberType = compiler.get_type(type.member_types[i]);

            if(memberType.basetype == spirv_cross::SPIRType::BaseType::Struct)
            {
                for (size_t j = 0; j < memberType.member_types.size(); j++)
                {
                    ShaderUniformMember member;
                    member.Name = compiler.get_member_name(memberType.self, static_cast<uint32_t>(j));
                    member.Size = compiler.get_declared_struct_member_size(memberType, static_cast<uint32_t>(j));
                    member.Offset = compiler.type_struct_member_offset(memberType, static_cast<uint32_t>(j));
                    block.Members.insert({member.Name, std::move(member)});
                }
            }
            else
            {
                ShaderUniformMember member;
                member.Name = compiler.get_member_name(type.self, static_cast<uint32_t>(i));
                member.Size = compiler.get_declared_struct_member_size(type, static_cast<uint32_t>(i));
                member.Offset = compiler.type_struct_member_offset(type, static_cast<uint32_t>(i));
            
                block.Members.insert({member.Name, std::move(member)});
            }
            
        }


        uint32_t set = compiler.get_decoration(resource.id, spv::DecorationDescriptorSet);
        uint32_t binding = compiler.get_decoration(resource.id, spv::DecorationBinding);

        block.Name = resource.name;
        block.Size = compiler.get_declared_struct_size(type);
        block.Bind = binding;
        block.Set = set;
        
        ans.UniformBlocks.insert({resource.name, block});
    }

    return ans;
}

ShaderTextureInputs ShaderDecoder::decodeTextures(std::vector<uint8>* binaryShader)
{
    ShaderTextureInputs ret;

    if(binaryShader->empty())
    {
        throw std::runtime_error("binaryShader is empty");
    }
    
    std::vector<uint32_t>& temp = reinterpret_cast<std::vector<uint32_t>&>(*binaryShader);
    spirv_cross::Compiler compiler(temp);

    auto getResource = [&ret, &compiler](
        const spirv_cross::Resource& resource,
        spirv_cross::SPIRType::BaseType type)
    {
        // 打印资源名称
        uint32_t binding = compiler.get_decoration(resource.id, spv::DecorationBinding);
        uint32_t set = compiler.get_decoration(resource.id, spv::DecorationDescriptorSet);

        ShaderTextureInput input;
        input.name = resource.name;
        input.bind = binding;
        input.set = set;
        input.type = type;

        ret.Members.insert({resource.name, input});
    };

    // 获取反射数据
    spirv_cross::ShaderResources resources = compiler.get_shader_resources();

    // 遍历所有的Image变量
    for (auto& resource : resources.separate_samplers)
    {
        const spirv_cross::SPIRType& type = compiler.get_type(resource.base_type_id);

        if(type.basetype == spirv_cross::SPIRType::Sampler)
        {
            getResource(resource, type.basetype);
        }
    }
    
    for (auto& resource : resources.separate_images)
    {
        const spirv_cross::SPIRType& type = compiler.get_type(resource.base_type_id);
        if (type.basetype == spirv_cross::SPIRType::SampledImage ||
            type.basetype == spirv_cross::SPIRType::Image)
        {
            getResource(resource, type.basetype);
        }
    }

    for (auto& resource : resources.sampled_images)
    {
        const spirv_cross::SPIRType& type = compiler.get_type(resource.base_type_id);
        if (type.basetype == spirv_cross::SPIRType::SampledImage ||
            type.basetype == spirv_cross::SPIRType::Image)
        {
            getResource(resource, type.basetype);
        }
    }
    
    return ret;
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
    if(!log.empty())
    {
        LOG(log);
    }
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

    ShaderTextureInputs = ShaderDecoder::decodeTextures(&data);

    ShaderUniformBufferBlocks.Log();
    
    ShaderTextureInputs.Log();

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

    for(auto& block :
        ShaderTextureInputs.Members)
    {
        VkDescriptorSetLayoutBinding ImageBinding = {};

        // 绑定点
        ImageBinding.binding = block.second.bind;
        // 类型
        switch (block.second.type)
        {
        case spirv_cross::SPIRType::Image:
            ImageBinding.descriptorType = VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE;
            break;
        case spirv_cross::SPIRType::SampledImage:
            ImageBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
            break;
        case spirv_cross::SPIRType::Sampler:
            ImageBinding.descriptorType = VK_DESCRIPTOR_TYPE_SAMPLER;
            break;
        default:
            continue;
        }
        // 数组数量
        ImageBinding.descriptorCount = 1;
        // 作用阶段
        ImageBinding.stageFlags = GetVkShaderStageFlagBits();
        // 纹理采样器
        ImageBinding.pImmutableSamplers = nullptr;
        
        out.emplace_back(ImageBinding);
    }
}
