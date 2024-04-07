// #include "GlobalUniformBufferManager.h"
//
// #include "Header.h"
// #include "Engine/Core/ShaderProgram.h"
// #include "Engine/Core/Camera/Camera.h"
//
// void GlobalUniformBuffer::RegistUniformBlock(ShaderProgram* program)
// {
//     std::vector<std::string> ss;
//     program->GetAllUniformBlockName(ss);
//     GLID BindPort = -1;
//     for (auto& s : ss)
//     {
//         
//         // 获取块索引
//         GLID blockIndex = program->GetUniformBlockIndex(s);
//         auto UniformBlockMapCache = UniformBlockMap.find(s);
//         if (UniformBlockMapCache == UniformBlockMap.end())
//         {
//             UniformBufferInfo info;
//             // 获取块大小
//             info.size = program->glGetActiveUniformBlockSize(blockIndex);
//             // 获取成员名和偏移
//             {
//                 GLint numUniforms = program->GetUniformBlockMemberNumbers(blockIndex);
//
//                 std::vector<GLID> Ids;
//                 program->GetUniformBlockMemberId(numUniforms, blockIndex, Ids);
//
//                 for (GLint i = 0; i < numUniforms; ++i) {
//                     std::string name = program->GetUniformBlockMemberName(Ids[i]);
//
//                     GLint size = program->GetUniformBlockMemberSize(Ids[i]);
//
//                     GLenum type = program->GetUniformBlockMemberType(Ids[i]);
//                     
//                     GLint offset = program->GetUniformBlockMemberOffset(Ids[i]);
//
//                     // LOG("Uniform ",uniformName," has offset ",offset,"\n");
//                     info.offsetMap.insert({
//                         name,
//                         {type, size, offset}
//                     });
//                 }
//             }
//
//             
//             glGenBuffers(1, &info.uniformBufferID);
//             glBindBuffer(GL_UNIFORM_BUFFER, info.uniformBufferID);
//             glBufferData(GL_UNIFORM_BUFFER, info.size, nullptr, GL_STATIC_DRAW);
//
//             LOG("Create New Uniform Buffer:", s, "ID:", info.uniformBufferID);
//             info.BindPort = (GLuint)UniformBlockMap.size();
//             BindPort = info.BindPort;
//             glBindBufferBase(GL_UNIFORM_BUFFER,
//                 info.BindPort, info.uniformBufferID);
//             RetrySetOperation(s, info);
//             UniformBlockMap.insert({s, std::move(info)});
//         }
//         else
//         {
//             const GLsizei size = program->glGetActiveUniformBlockSize(blockIndex);
//
//             // 简单检查一下
//             if (UniformBlockMapCache->second.size != size)
//             {
//                 LOG("Error: Uniform Size Not Match", s);
//             }
//             BindPort = UniformBlockMapCache->second.BindPort;
//         }
//         LOG("Bind Uniform", s, program->GetName(), BindPort);
//         glUniformBlockBinding(program->getProgramID(),
//             blockIndex, BindPort);
//     }
// }
//
// void GlobalUniformBuffer::RetrySetOperation(const std::string& name,
//     const UniformBufferInfo& info)
// {
//     auto m = MissingBufferSetOperationCache.find(name);
//     if(m == MissingBufferSetOperationCache.end())
//     {
//         return;
//     }
//
//     glBindBuffer(GL_UNIFORM_BUFFER,info.uniformBufferID);
//     for(SetUniformCommand& c : m->second)
//     {
//         if(c.memberName.empty())
//         {
//             glBufferSubData(GL_UNIFORM_BUFFER,
//                 c.Offset,
//                 c.Data.size(), c.Data.data());
//         }else
//         {
//             auto offset = info.offsetMap.find(c.memberName);
//             if(info.offsetMap.end() == offset)
//             {
//                 LOG(name, "has no nummber named ", c.memberName);
//                 continue;
//             }
//             glBufferSubData(GL_UNIFORM_BUFFER,
//                 offset->second.offset,
//                 c.Data.size(), c.Data.data());
//         }
//     }
//
// #ifdef CHECK_BUFFER_BLOCK
//     glGetBufferSubData(GL_UNIFORM_BUFFER, c.Offset, c.Data.size(), c.Data.data());
//
//     LOG(c.Offset);
//     LOG(*(glm::mat4*)c.Data.data());
//     LOG(Camera::GetCamera()->GetCameraProjection());
//     
// #endif
//     MissingBufferSetOperationCache.erase(name);
// }
//
// GlobalUniformBuffer* GlobalUniformBuffer::GetInstance()
// {
//     static GlobalUniformBuffer ins;
//
//     return &ins;
// }
