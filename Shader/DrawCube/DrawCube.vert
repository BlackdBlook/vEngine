#version 450

layout(location = 0) out vec3 FragPos;
layout(location = 1) out vec3 Normal;
layout(location = 2) out vec2 TexCoords;


layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoord;

layout(set = 0, binding = 0) uniform Camera
{
    mat4 model;
    mat4 u_View;
    mat4 u_Projection;
};
    
void main() {

    FragPos = vec3(model * vec4(pos, 1.0));
    Normal = mat3(transpose(inverse(model))) * normal;
    TexCoords = texCoord;

    gl_Position = u_Projection * u_View * vec4(FragPos, 1.0);

}
 