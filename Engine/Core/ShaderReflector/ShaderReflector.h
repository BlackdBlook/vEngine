#pragma once
#include <vector>
#include "Header.h"
#include "Engine/Core/Container/Name.h"
#include "LogPrinter/Log.h"
#include "ThirdParty/SPIRV-Cross/spirv_cross.hpp"


namespace shader
{
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
        Container::Name Name;
        uint32 ElementSize;
        uint32 Set;
        uint32 Bind;
        std::vector<uint32> Array_length;
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

    struct ShaderUniformBufferBlocks
    {
        UniformBlockMap UniformBlocks;
        UniformMemberMap UniformMembers;
        void Log();
    };


    class ShaderReflector
    {
        using BufferBlocksPtr = SPtr<ShaderUniformBufferBlocks>;
        
        spirv_cross::Compiler compiler;
        void prossesMemberName(string parentName, const spirv_cross::SPIRType& parentType
                               , UniformMemberMap& out);
    public:
        ShaderReflector(const std::vector<uint8>& shaderByteCode);

        SPtr<ShaderUniformBufferBlocks> GetUniformBufferBlocks();
    };
    
}
