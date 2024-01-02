#include "Shader.h"

#include <filesystem>

#include "Engine/vEngine.h"
#include "Engine/SubSystem/AssetSystem/AssetSystem.h"
#include "Engine/Toolkit/FileToolKit/FileToolKit.h"

VkShaderModule createShaderModule(const std::vector<uint8>& code) {
    VkShaderModuleCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = code.size();
    createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

    VkShaderModule shaderModule;
    if (vkCreateShaderModule(GlobalVkLogicDevice, &createInfo, nullptr, &shaderModule) != VK_SUCCESS) {
        throw std::runtime_error("failed to create shader module!");
    }
    return shaderModule;
}

vShader::vShader(const char* Name, ShaderType type)
{
    string fileName = Name;

    switch (type) {
    case ShaderType::Vertex:
        fileName += ".vert.spv";
        break;
    case ShaderType::Fragment:
        fileName += ".frag.spv";
        break;
    }
    
    auto Path = AssetSystem::GetInstance()->GetFilePathByName(fileName);

    auto data = FileToolKit::ReadFileAsBinary(Path);

    vkshadermodule = createShaderModule(data);
    
}

vShader::~vShader()
{
    vkDestroyShaderModule(GlobalVkLogicDevice, vkshadermodule, nullptr);
}
