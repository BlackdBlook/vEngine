
#include "../CommonFunction.hlsli"
#include "../CommonStruct.hlsli"
#include "../CommonDef.hlsli"

// 创建一个ConstantBuffer来存储PointLight数组
// 使用vk::binding修饰符来指定Vulkan中的绑定和集合索引


DefaultVSOutput VS(DefaultVSInput input)
{
    return CommonVS(input);
}

struct Material {
    float shininess;
};

Texture2D texture0 : register(t3, space0);
SamplerState sampler0 : register(s4, space0);

[[vk::binding(5, 0)]]
ConstantBuffer<Material> material;


[[vk::binding(6, 0)]]
ConstantBuffer<PointLight> pointLight;


// calculates the color when using a point light.
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, float2 TexCoords)
{
    // ambient
    vec3 ambient = texture0.Sample(sampler0, TexCoords).xyz;
  	
    // diffuse 
    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(light.position - fragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * texture0.Sample(sampler0, TexCoords).xyz;  
    
    // specular
    vec3 view = normalize(viewDir - fragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(view, reflectDir), 0.0), material.shininess);
    vec3 specular = spec * texture0.Sample(sampler0, TexCoords).xyz;  
    
    // attenuation
    float distance    = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linears * distance + light.quadratic * (distance * distance));    

    ambient  *= attenuation;  
    diffuse   *= attenuation;
    specular *= attenuation;   
        
    return ambient + diffuse + specular;
    
}

float4 PS(DefaultVSOutput input) : SV_TARGET
{
    vec3 viewDir = normalize(Global.u_CameraPos - input.FragPos.xyz);
    
    float3 result = CalcPointLight(pointLight, input.Normal, input.FragPos.xyz, viewDir, input.TexCoords);
      
    return float4(result, 1.0f);
}
