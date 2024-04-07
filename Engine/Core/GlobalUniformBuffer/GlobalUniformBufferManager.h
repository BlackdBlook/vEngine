// #pragma once
// #include <set>
// #include <unordered_map>
//
// #include "Header.h"
// #include "ToolKit/Typedef.h"
//
// class ShaderProgram;
//
// struct UniformBufferMemberInfo
// {
// public:
//     GLenum type;
//     GLint size;
//     GLint offset;
// };
//
// struct UniformBufferInfo
// {
//     GLsizei size;
//     GLID uniformBufferID;
//     GLID BindPort;
//
//     std::unordered_map<std::string, UniformBufferMemberInfo> offsetMap;
//     
//     UniformBufferInfo() = default;
//     UniformBufferInfo& operator=(const UniformBufferInfo& other)
//     {
//         size = other.size;
//         uniformBufferID = other.uniformBufferID;
//         BindPort = other.BindPort;
//         offsetMap = other.offsetMap;
//         return *this;
//     }
//     UniformBufferInfo(const UniformBufferInfo& other)
//     {
//         size = other.size;
//         uniformBufferID = other.uniformBufferID;
//         BindPort = other.BindPort;
//         offsetMap = other.offsetMap;
//     }
//     UniformBufferInfo(UniformBufferInfo&& other) noexcept
//     {
//         size = other.size;
//         uniformBufferID = other.uniformBufferID;
//         BindPort = other.BindPort;
//         offsetMap = std::move(other.offsetMap);
//     }
// };
//
// struct SetUniformCommand
// {
//     std::string name;
//     GLintptr Offset;
//     std::string memberName;
//     std::vector<uint8> Data;
//     SetUniformCommand() = default;
//     SetUniformCommand(const SetUniformCommand& other) = delete;
//
//     SetUniformCommand(SetUniformCommand&& other) noexcept
//     {
//         name = std::move(other.name);
//         memberName = std::move(other.memberName);
//         Offset = other.Offset;
//         Data = std::move(other.Data);
//     }
// };
//
// class GlobalUniformBuffer
// {
//     std::unordered_map<std::string, UniformBufferInfo> UniformBlockMap {};
//
//     std::unordered_map<std::string, std::vector<SetUniformCommand>> MissingBufferSetOperationCache;
// public:
//
//     void RegistUniformBlock(ShaderProgram* program);
//
//     template<typename T>
//     void SetUniformBuffer(string&& Name, string&& memberName, T* Data)
//     {
//         auto target = UniformBlockMap.find(Name);
//         if(target == UniformBlockMap.end())
//         {
//             ERROR("Error Uniform Buffer Not Find:", Name);
//             CacheMissingBufferSetOperation<T>(
//                 std::forward<string>(Name), Data, std::forward<string>(memberName));
//             return;
//         }
//         auto targetMember =
//             target->second.offsetMap.find(memberName);
//         if(targetMember == target->second.offsetMap.end())
//         {
//             ERROR("Error Uniform Buffer Not Find:", Name);
//             CacheMissingBufferSetOperation<T>(
//                 std::forward<string>(Name), Data, std::forward<string>(memberName));
//             return;
//         }
//
//         // if(target->second.size != sizeof(T))
//         // {
//         //     LOG("Error Uniform Buffer Size Not Match",
//         //         Name, target->second.size, sizeof(T));
//         //     return;
//         // }
//         // LOG("Set Uniform block data", Name);
//
//         // LOG("SetUniformBuffer", Name, memberName);
//         
//         glBindBuffer(GL_UNIFORM_BUFFER,target->second.uniformBufferID);
//         glBufferSubData(GL_UNIFORM_BUFFER, targetMember->second.offset, sizeof(T), Data);
//     }
//
//     template<typename T>
//     void CacheMissingBufferSetOperation(string&& Name,void* Data, GLintptr Offset)
//     {
//         SetUniformCommand command;
//         command.name = std::string{Name};
//         command.Offset = Offset;
//         size_t s = sizeof(T);
//         command.Data = std::vector<uint8>(s);
//         memcpy(command.Data.data(), Data, sizeof(T));
//
//         std::string ns = Name;
//         auto list = MissingBufferSetOperationCache.find(ns);
//         if(list == MissingBufferSetOperationCache.end())
//         {
//             MissingBufferSetOperationCache.insert({ns,
//             std::vector<SetUniformCommand>(0)});
//             list = MissingBufferSetOperationCache.find(ns);
//         }
//         
//         list->second.emplace_back(std::move(command));
//     }
//     
//     template<typename T>
//     void CacheMissingBufferSetOperation(string&& Name,
//         void* Data, string&& memberName)
//     {
//         SetUniformCommand command;
//         command.name = std::string{Name};
//         command.memberName = memberName;
//         size_t s = sizeof(T);
//         command.Data = std::vector<uint8>(s);
//         memcpy(command.Data.data(), Data, sizeof(T));
//
//         std::string ns = Name;
//         auto list = MissingBufferSetOperationCache.find(ns);
//         if(list == MissingBufferSetOperationCache.end())
//         {
//             MissingBufferSetOperationCache.insert({ns,
//                 std::vector<SetUniformCommand>(0)});
//             list = MissingBufferSetOperationCache.find(ns);
//         }
//         
//         list->second.emplace_back(std::move(command));
//     }
//
//     void RetrySetOperation(const std::string& name,
//         const UniformBufferInfo& info);
//
//     static GlobalUniformBuffer* GetInstance();
// };
//
// template<typename T>
// static void SetGlobalUniformBuffer(string&& Name, string&& Member, T&& Data)
// {
//     GlobalUniformBuffer::GetInstance()->SetUniformBuffer(
//         std::forward<string>(Name), std::forward<string>(Member), &Data);
// }
