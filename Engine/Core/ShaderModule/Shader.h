#pragma once
#include <unordered_map>
#include <vector>
#include <vulkan/vulkan_core.h>
#include "Header.h"
#include "ThirdParty/SPIRV-Cross/spirv_common.hpp"

enum class ShaderType
{
    Vertex,
    Fragment
};

enum class ShaderCodeType : uint8
{
    HLSL,
    GLSL,
};

struct ShaderUniformMember
{
    string Name;
    size_t Size;
    size_t Offset;
    std::string Log();
};

struct ShaderUniformBufferBlock
{
    string Name;
    size_t Size;
    uint32 Set;
    uint32 Bind;
    std::unordered_map<std::string, ShaderUniformMember> Members;
    std::string Log();
};

struct ShaderUniformBufferBlocks
{
    std::unordered_map<string, ShaderUniformBufferBlock> UniformBlocks;
    void Log();
};

struct ShaderTextureInput
{
    string name;
    uint32 bind;
    uint32 set;
    spirv_cross::SPIRType::BaseType type;

    VkDescriptorType GetDescriptorType();

    std::string Log();
};

struct ShaderTextureInputs
{
    std::unordered_map<string, ShaderTextureInput> Members;
    void Log();
};

class ShaderDecoder
{
public:
    static ShaderUniformBufferBlocks decodeUniformBuffer(std::vector<uint8>* binaryShader);
    static ShaderTextureInputs decodeTextures(std::vector<uint8>* binaryShader);
};


class vShader
{
    VkShaderModule vkshadermodule;

    VkShaderStageFlagBits GetVkShaderStageFlagBits();

    ShaderType type;
    
public:
    ShaderUniformBufferBlocks ShaderUniformBufferBlocks;

    ShaderTextureInputs ShaderTextureInputs;
    
    VkShaderModule GetShaderModule()
    {
        return vkshadermodule;
    }

    string EntryPointFunctionName;
    
    vShader(const char* Name, ShaderType type, ShaderCodeType codeType = ShaderCodeType::HLSL);
    
    ~vShader();

    VkPipelineShaderStageCreateInfo GetStageInfo();
    
    void FillDescriptorSetLayoutBinding(std::vector<VkDescriptorSetLayoutBinding>& out);
};
