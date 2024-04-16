#pragma once
#include <vector>

#include "Engine/Core/ShaderModule/Shader.h"
#include "Engine/Core/UniformBuffer/UniformBuffer.h"

 
class GlobalUniformBufferManager
{
public:
    
    UniformBuffer buffer;

    std::unordered_map<string, size_t> MemberOffset;

    

private:
    
    void Init(SPtr<vShader> shader);
};
