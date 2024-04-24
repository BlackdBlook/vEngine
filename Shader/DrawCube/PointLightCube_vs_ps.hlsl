
#include "../CommonFunction.hlsli"
#include "../CommonStruct.hlsli"

// 创建一个ConstantBuffer来存储PointLight数组
// 使用vk::binding修饰符来指定Vulkan中的绑定和集合索引
[[vk::binding(2, 0)]]
ConstantBuffer<PointLight> pointLightBuffer[4];

Texture2D texture0 : register(t3, space0);
SamplerState sampler0 : register(s4, space0);

DefaultVSOutput VS(DefaultVSInput input)
{
    return CommonVS(input);
}

float4 PS(DefaultVSOutput input) : SV_TARGET
{
    float4 pos = texture0.Sample(sampler0, input.TexCoords);;
    pos += float4(pointLightBuffer[0].position, 1);
    pos += float4(pointLightBuffer[1].position, 1);
    pos += float4(pointLightBuffer[2].position, 1);
    pos += float4(pointLightBuffer[3].position, 1);

    return pos;
}
