
#include "../CommonFunction.hlsli"
#include "../CommonStruct.hlsli"


// 创建一个ConstantBuffer来存储PointLight数组
// 使用vk::binding修饰符来指定Vulkan中的绑定和集合索引
[[vk::binding(2, 0)]]
ConstantBuffer<PointLight> pointLightBuffer[4];

DefaultVSOutput VS(DefaultVSInput input)
{
    return CommonVS(input);
}

float4 PS(DefaultVSOutput input) : SV_TARGET
{
    return float4(pointLightBuffer[0].position, 1.0);
}
