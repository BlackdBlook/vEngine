#pragma once
#include <unordered_map>
#include <vector>
#include <vulkan/vulkan_core.h>
#include "Header.h"
#include "Engine/Core/ShaderReflector/ShaderReflector.h"
#include "ThirdParty/SPIRV-Cross/spirv_common.hpp"

class ITexture;

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

struct ShaderTextureInput
{
    string name;
    uint32 bind;
    uint32 set;
    spirv_cross::SPIRType::BaseType type;
    spv::Dim dim;

    VkDescriptorType GetDescriptorType();

    SPtr<ITexture> CreateTextureObject();

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
    
    static ShaderTextureInputs DecodeTextures(std::vector<uint8>* binaryShader);
};


class vShader
{
    VkShaderModule vkshadermodule;

    VkShaderStageFlagBits GetVkShaderStageFlagBits();

    ShaderType type;

public:
    
    void CountTextureInputNum(uint32& textureNum, uint32& samplerNum, uint32& CombindImageNum);

    ShaderTextureInputs ShaderTextureInputs;
    
    VkShaderModule GetShaderModule()
    {
        return vkshadermodule;
    }

    string EntryPointFunctionName;
    
    vShader(const char* Name, ShaderType type, struct ShaderUniformBufferBlocks* blocks,
            ShaderCodeType codeType = ShaderCodeType::HLSL);
    
    ~vShader();

    VkPipelineShaderStageCreateInfo GetStageInfo();
    
    void FillDescriptorSetLayoutBinding(std::vector<VkDescriptorSetLayoutBinding>& out);
};
