#include "GlobalUniformBufferManager.h"

void GlobalUniformBufferManager::Init(SPtr<vShader> shader)
{
    auto block = shader->ShaderUniformBufferBlocks.UniformBlocks.find("GlobalUniformBuffer");

    if(block != shader->ShaderUniformBufferBlocks.UniformBlocks.end())
    {
        buffer.Init(block->second.Size, block->second.Name, block->second.Bind);
        for(auto& member : block->second.Members)
        {
            MemberOffset[member.first] = member.second.Offset;
        }
    }
}
