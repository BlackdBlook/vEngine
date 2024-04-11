#pragma once
#include <unordered_map>
#include <vector>
#include <vulkan/vulkan_core.h>
#include "Header.h"

enum class ShaderType
{
    Vertex,
    Fragment
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

class ShaderDecoder
{
public:
    static ShaderUniformBufferBlocks decodeUniformBuffer(std::vector<uint8>* binaryShader);
};


class vShader
{
    VkShaderModule vkshadermodule;

    VkShaderStageFlagBits GetVkShaderStageFlagBits();

    ShaderType type;
    
public:
    ShaderUniformBufferBlocks ShaderUniformBufferBlocks;
    
    VkShaderModule GetShaderModule()
    {
        return vkshadermodule;
    }
    vShader(const char* Name, ShaderType type);
    
    ~vShader();

    VkPipelineShaderStageCreateInfo GetStageInfo();
    
    void FillDescriptorSetLayoutBinding(std::vector<VkDescriptorSetLayoutBinding>& out);
};
