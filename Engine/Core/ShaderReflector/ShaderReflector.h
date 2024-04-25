#pragma once
#include <vector>
#include "Header.h"
#include "Engine/Core/Container/Name.h"
#include "Engine/Core/ShaderModule/Shader.h"
#include "LogPrinter/Log.h"
#include "ThirdParty/SPIRV-Cross/spirv_cross.hpp"


enum class ShaderType;

struct ShaderUniformMember
{
    Container::Name Name;
    size_t Size;
    size_t Offset;

    bool operator==(const ShaderUniformMember& other);
    bool operator!=(const ShaderUniformMember& other);

    std::string Log();
};

struct ShaderUniformBufferBlock
{
    bool IsGlobalUnifomrBuffer = false;
    Container::Name Name;
    uint32 ElementSize;
    uint32 Set;
    uint32 Bind;
    std::vector<uint32> Array_length;
    
    VkShaderStageFlagBits shaderType;
    // std::unordered_map<Container::Name, ShaderUniformMember> Members;

    bool operator==(const ShaderUniformBufferBlock& other);
    bool operator!=(const ShaderUniformBufferBlock& other);
    uint32 CaclBufferSize();
    uint32 CaclBufferElementNum();
    uint32 GetElementOffset();
    std::string Log();
};

using UniformMemberMap = std::unordered_map<Container::Name, ShaderUniformMember>;
using UniformBlockMap = std::unordered_map<Container::Name, ShaderUniformBufferBlock>;
using UniformMemberBlockMap = std::unordered_map<Container::Name, Container::Name>;

struct GlobalUniformBufferInfo
{
    uint32 ElementSize;
    UniformMemberMap Members;
};


struct ShaderUniformBufferBlocks
{
    UniformBlockMap UniformBlocks;
    UniformMemberMap UniformMembers;
    UniformMemberBlockMap UniformMemberInBlocks;
    void Log();
};


class ShaderReflector
{
    using BufferBlocksPtr = SPtr<ShaderUniformBufferBlocks>;
    VkShaderStageFlagBits ShaderStageFlagBits;
    spirv_cross::Compiler compiler;
    void prossesMemberName(string parentName, const spirv_cross::SPIRType& parentType
                           , UniformMemberMap& out);

    void processBlock(string& name, const spirv_cross::SPIRType& type, UniformMemberMap& out);
    
public:
    ShaderReflector(const std::vector<uint8>& shaderByteCode, VkShaderStageFlagBits shaderStageFlagBits);

    bool MergeToUniformBufferBlocks(ShaderUniformBufferBlocks* blocks);
};
    

