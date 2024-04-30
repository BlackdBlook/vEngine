
#include "../GlobalUniformBuffer.hlsli"

struct VSInput
{
    [[vk::location(0)]] float3 pos : POSITION0;
};

struct SkyboxView
{
    float4x4 u_View;
    float4x4 u_Projection;
};

[[vk::binding(1, 0)]]
ConstantBuffer<SkyboxView> SkyboxViewBuffer;


TextureCube texture0 : register(t2, space0);
SamplerState sampler0 : register(s3, space0);

struct VSOutput    
{
    float4 Position : SV_POSITION;
    float3 TexCoords : TEXCOORD;
};        

VSOutput VS(VSInput input)
{
    VSOutput output;
    
    output.TexCoords = input.pos;
    output.Position = mul(SkyboxViewBuffer.u_Projection,
        mul(SkyboxViewBuffer.u_View, float4(input.pos, 1.0)));

    return output;
}

float4 PS(VSOutput input) : SV_TARGET
{
    float4 tex = texture0.Sample(sampler0, input.TexCoords);

    return tex;
}
