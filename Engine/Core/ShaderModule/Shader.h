#pragma once
#include <vulkan/vulkan_core.h>


enum class ShaderType
{
    Vertex,
    Fragment
};

class vShader
{
    VkShaderModule vkshadermodule;

    VkShaderStageFlagBits GetVkShaderStageFlagBits();

    ShaderType type;
    
public:
    VkShaderModule GetShaderModule()
    {
        return vkshadermodule;
    }
    vShader(const char* Name, ShaderType type);
    
    ~vShader();

    VkPipelineShaderStageCreateInfo GetStageInfo();
};
