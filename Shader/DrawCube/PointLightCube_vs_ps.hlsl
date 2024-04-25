
#include "../CommonFunction.hlsli"
#include "../CommonStruct.hlsli"
#include "../CommonDef.hlsli"

// 创建一个ConstantBuffer来存储PointLight数组
// 使用vk::binding修饰符来指定Vulkan中的绑定和集合索引


DefaultVSOutput VS(DefaultVSInput input)
{
    return CommonVS(input);
}

struct DirLight {
    float strength;
    
    vec3 direction;
    vec3 color;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct Material {
    float shininess;
};

Texture2D texture0 : register(t3, space0);
SamplerState sampler0 : register(s4, space0);

[[vk::binding(5, 0)]]
ConstantBuffer<Material> material;


[[vk::binding(6, 0)]]
ConstantBuffer<DirLight> dirLight;


// calculates the color when using a directional light.
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir, vec2 TexCoords)
{
    vec3 lightDir = normalize(-light.direction);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // combine results
    vec3 ambient = light.ambient * vec3(texture0.Sample(sampler0, TexCoords).xyz);
    vec3 diffuse = light.diffuse * diff * vec3(texture0.Sample(sampler0, TexCoords).xyz);
    vec3 specular = light.specular * spec * vec3(texture0.Sample(sampler0, TexCoords).xyz);

    vec3 lighting = (ambient * (diffuse + specular));
    //    vec3 lighting = (ambient + (diffuse + specular));
    return lighting * light.color * light.strength;
}

float4 PS(DefaultVSOutput input) : SV_TARGET
{
    vec3 viewDir = normalize(float4(Global.u_CameraPos, 1) - input.FragPos).xyz;
    
    float3 result = CalcDirLight(dirLight, input.Normal, viewDir, input.TexCoords);
    // float3 result = CalcDirLight(dirLight, input.Normal, viewDir, input.TexCoords);

    result += float3(3000,3000,3000);

    
    return float4(result, 1.0f);
}
