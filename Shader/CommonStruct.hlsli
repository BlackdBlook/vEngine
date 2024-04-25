#pragma once
struct DefaultVSInput
{
    [[vk::location(0)]] float3 pos : POSITION0;
    [[vk::location(1)]] float3 normal : COLOR0;
    [[vk::location(2)]] float2 texCoord : TEXCOORD;
};

struct CommonModelBuffer
{
    float4x4 model;
};

#ifdef EnableCommonModelBuffer
[[vk::binding(1, 0)]]
ConstantBuffer<CommonModelBuffer> model;
#endif

struct DefaultVSOutput    
{
    float4 Position : SV_POSITION;
    float4 FragPos : POSITION;
    float3 Normal : NORMAL;
    float2 TexCoords : TEXCOORD;
};        

struct PointLight {
    float strength;
    
    float3 position;
    float3 color;
    float constant;
    float linears;
    float quadratic;
};