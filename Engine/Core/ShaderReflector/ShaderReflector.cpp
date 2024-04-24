#include "ShaderReflector.h"

#include "Engine/VulkanHelper/VkHelper.h"
#include "LogPrinter/Log.h"
#include "ThirdParty/StringFormater/StringFormater.h"
using namespace shader;

/// Buffer命名规则：
/// 1. 一个Buffer的名字与Shader中相同
/// 2. 如果Buffer是个数组，那么Buffer的名字是"数组的名字[下标]"
/// 3. 成员同理
/// 举个栗子：
/// struct Test2
/// {
///     float4 position;
///     float4 color[2];
/// };
/// 
/// struct Test1
/// {
///     Test2 test2[2];
/// };
/// [[vk::binding(3, 0)]]
/// ConstantBuffer<Test1> Test1[4];
///
/// 显而易见的访问ID有：
/// Test1[2].test2[1].color[0]
/// Test1[2].test2[0].position
///


bool ShaderUniformMember::operator==(const ShaderUniformMember& other)
{
    return Name == other.Name && Size == other.Size && Offset == other.Offset;
}

bool ShaderUniformMember::operator!=(const ShaderUniformMember& other)
{
    return !(*this == other);
}

std::string ShaderUniformMember::Log()
{
    return "Name:" + Name.ToString() + " ,ElementSize:" + std::to_string(Size) + " ,Offset:" + std::to_string(Offset);
}

bool ShaderUniformBufferBlock::operator==(const ShaderUniformBufferBlock& other)
{
    return Name == other.Name && ElementSize == other.ElementSize && Set == other.Set && Bind == other.Bind;
}

bool ShaderUniformBufferBlock::operator!=(const ShaderUniformBufferBlock& other)
{
    return !(*this == other);
}

uint32 ShaderUniformBufferBlock::CaclBufferSize()
{
    // 一个对象的大小
    return GetElementOffset() * CaclBufferElementNum();
}

uint32 ShaderUniformBufferBlock::CaclBufferElementNum()
{
    uint32 ret = 1;
    if(Array_length.empty())
    {
        // 不是一个数组，直接返回
        return ret;
    }
    for(uint32& i : Array_length)
    {
        // 乘以对象的数量
        ret *= i;
    }
    return ret;
}

uint32 ShaderUniformBufferBlock::GetElementOffset()
{
    return VkHelperInstance->GetUniformBufferOffsetByElementSize(ElementSize);
}

std::string ShaderUniformBufferBlock::Log()
{
    string ans = "Uniform Block " + Name.ToString() + ",ElementSize:" + std::to_string(ElementSize) +
        ",Set:" + std::to_string(Set) + ",Bind:" + std::to_string(Bind) + '\n';
    
    // for(auto& m : Members)
    // {
    //     ans += '\t' + m.second.Log() + '\n';
    // }

    return ans;
}

void ShaderUniformBufferBlocks::Log()
{
#ifdef _DEBUG
    string log;
    for(auto& b : UniformBlocks)
    {
        log += b.second.Log() + '\n';
    }
    if(!log.empty())
    {
        LOG(log);
    }
#endif
}

ShaderReflector::ShaderReflector(const std::vector<uint8>& shaderByteCode)
    :compiler(reinterpret_cast<const std::vector<uint32>&>(shaderByteCode))
{
    
}

void ShaderReflector::prossesMemberName(
    string parentName
    , const spirv_cross::SPIRType& parentType
    , UniformMemberMap& out)
{
    for (uint32 i = 0;i < parentType.member_types.size();i++)
    {
        auto memberTypeId = parentType.member_types[i];
        const spirv_cross::SPIRType& memberType = compiler.get_type(memberTypeId);
        string memberName = compiler.get_member_name(parentType.self, i);
        if(memberType.basetype == spirv_cross::SPIRType::BaseType::Struct)
        {
            // 继续找基本类型
            if(memberType.array.empty())
            {
                // 这是个结构体
                string memberPrefix = Format(parentName, memberName, '.');
                prossesMemberName(memberPrefix, memberType, out);
            }
            else
            {
                // 这是个数组，i是这个数组的长度
                for(uint32 j : memberType.array)
                {
                    for(uint32 h = 0;h < j ; h++)
                    {
                        string memberPrefix = Format(parentName, memberName, '[', h, ']', '.');
                        prossesMemberName(memberPrefix, memberType, out);
                    }
                }
            }
        }
        else
        {
            if(memberType.array.empty())
            {
                // 这是个基本类型
                // 保存这个成员
                Container::Name selfName = Format(parentName, memberName);

                ShaderUniformMember member;

                member.Name = compiler.get_name(memberTypeId);

                member.Size = compiler.get_declared_struct_member_size(parentType, i);

                member.Offset = compiler.type_struct_member_offset(parentType, i);
            
                out.insert({selfName, std::move(member)});
            }
            else
            {
                // 这是个数组，i是这个数组的长度
                for(uint32 j : memberType.array)
                {
                    for(uint32 h = 0;h < j ; h++)
                    {
                        // 这是个基本类型
                        // 保存这个成员
                        Container::Name selfName = Format(parentName, memberName, '[', h, ']');

                        ShaderUniformMember member;

                        member.Name = compiler.get_name(memberTypeId);

                        member.Size = compiler.get_declared_struct_member_size(parentType, i);

                        member.Offset = compiler.type_struct_member_offset(parentType, i);
            
                        out.insert({selfName, std::move(member)});
                    }
                }
            }
        }
    }
}

SPtr<ShaderUniformBufferBlocks> ShaderReflector::GetUniformBufferBlocks()
{
    BufferBlocksPtr ret = NewSPtr<ShaderUniformBufferBlocks>();

    // 获取反射数据
    spirv_cross::ShaderResources resources = compiler.get_shader_resources();

    // 遍历所有的uniform变量
    for (auto& resource : resources.uniform_buffers)
    {
        ShaderUniformBufferBlock block;
        // 块的名字
        string name = compiler.get_name(resource.id);
        const spirv_cross::SPIRType& type = compiler.get_type(resource.type_id);
        if(!type.array.empty())
        {
            // 这是个数组，i是这个数组的长度
            for(uint32 i : type.array)
            {
                for(uint32 j = 0; j < i; j++)
                {
                    string memberPrefix = Format(name, '[', j, "].");
                    prossesMemberName(memberPrefix, type, ret->UniformMembers);
                }
            }
        }
        else
        {
            // uniform_buffers拿到的类型必然是个结构体
            // 遍历uniform变量的所有成员
            
            string memberPrefix = Format(name, '.');
            prossesMemberName(memberPrefix, type, ret->UniformMembers);
        }

        block.Array_length = {type.array.begin(), type.array.end()};
        block.Name = name;
        block.ElementSize = compiler.get_declared_struct_size(type);
        block.Set = compiler.get_decoration(resource.id, spv::Decoration::DecorationDescriptorSet);
        block.Bind = compiler.get_decoration(resource.id, spv::Decoration::DecorationBinding);
        
        ret->UniformBlocks.insert({
            compiler.get_name(resource.id),
            std::move(block)
        });
    }


    return ret;
}
