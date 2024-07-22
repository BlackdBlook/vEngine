#include "./PostProcessing.hlsli"

#include "../GlobalUniformBuffer.hlsli"
#include "../CommonDef.hlsli"

Texture2D texture0 : register(t1, space0);
SamplerState sampler0 : register(s2, space0);

struct VSOutput    
{
    float4 Position : SV_POSITION;
    float2 TexCoords : TEXCOORD;
};        

VSOutput VS(uint VertexIndex : SV_VertexID)
{
    float2 location[6] = {
        float2(-1.0,     -1.0),
        float2(1.0,     1.0),
        float2(-1.0,    1.0),
        
        float2(-1.0,    -1.0),
        float2(1.0,     -1.0),
        float2(1.0,     1.0),
    }; 

    float2 uv[6] = {
        float2(1,     0),
        float2(0,     1),
        float2(1,     1), 
        
        float2(1,     0),
        float2(0,     0),  
        float2(0,     1),
    };
    
    VSOutput output;
    
    output.TexCoords = uv[VertexIndex];
    output.Position = float4(location[VertexIndex], 0, 1);

    return output;
}

float4 PS(VSOutput input) : SV_TARGET
{
    float4 tex = texture0.Sample(sampler0, input.TexCoords);
 
    return tex;
}
