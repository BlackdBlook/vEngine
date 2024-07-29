
#include "../GlobalUniformBuffer.hlsli"

struct VSInput
{
    [[vk::location(0)]] float3 pos : POSITION0;
};

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
    float3x3 viewTemp = (float3x3)Global.u_View;
    float4x4 view2 = float4x4(
        viewTemp._m00_m01_m02, 0,
        viewTemp._m10_m11_m12, 0,
        viewTemp._m20_m21_m22, 0,
        0, 0, 0, 1
    );

    
    output.Position = mul(Global.u_Projection,
        mul(view2, float4(input.pos, 1.0)));

    return output;
}

float4 PS(VSOutput input) : SV_TARGET
{
    float4 tex = texture0.Sample(sampler0, input.TexCoords);

    return tex;
} 
