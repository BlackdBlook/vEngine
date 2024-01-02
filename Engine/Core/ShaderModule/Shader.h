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
public:
    VkShaderModule GetShaderModule()
    {
        return vkshadermodule;
    }
    vShader(const char* Name, ShaderType type);
    ~vShader();
};
