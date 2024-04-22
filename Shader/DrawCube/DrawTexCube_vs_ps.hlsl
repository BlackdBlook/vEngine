
#include "../GlobalUniformBuffer.hlsli"

struct VSInput
{
[[vk::location(0)]] float3 pos : POSITION0;
[[vk::location(1)]] float3 normal : COLOR0;
[[vk::location(2)]] float2 texCoord : TEXCOORD;
};

struct ModelBuffer
{
	float4x4 model;
};

[[vk::binding(1, 0)]]
ConstantBuffer<ModelBuffer> model;

Texture2D texture0 : register(t2, space0);
SamplerState sampler0 : register(s3, space0);

struct VSOutput    
{
	float4 Position : SV_POSITION;
	float4 FragPos : POSITION;
	float3 Normal : NORMAL;
	float2 TexCoords : TEXCOORD;
};        

float4x4 inverse(float4x4 m)
{
    float det = determinant(m);
    float3x3 adj;
    float3x3 m3x3 = (float3x3)m; // Extract 3x3 submatrix
    adj._m00_m01_m02 = cross(m3x3._m10_m11_m12, m3x3._m20_m21_m22);
    adj._m10_m11_m12 = cross(m3x3._m20_m21_m22, m3x3._m00_m01_m02);
    adj._m20_m21_m22 = cross(m3x3._m00_m01_m02, m3x3._m10_m11_m12);
    float3x3 inv = transpose(adj) / det;
    // Convert back to 4x4 matrix
    float4x4 inv4x4;
    inv4x4._m00_m01_m02_m03 = float4(inv._m00_m01_m02, 0.0);
    inv4x4._m10_m11_m12_m13 = float4(inv._m10_m11_m12, 0.0);
    inv4x4._m20_m21_m22_m23 = float4(inv._m20_m21_m22, 0.0);
    inv4x4._m30_m31_m32_m33 = float4(0.0, 0.0, 0.0, 1.0);
    return inv4x4;
}

VSOutput VS(VSInput input)
{
    VSOutput output;
    output.FragPos = mul(model.model, float4(input.pos , 1.0));
	
    float4x4 modelInverseTranspose = transpose(inverse(model.model));
    float3 transformedNormal = mul(modelInverseTranspose, float4(input.normal, 0.0)).xyz;
    output.Normal = transformedNormal;
	
    output.TexCoords = input.texCoord;
	
    float4 pos = output.FragPos;
    pos = mul(GlobalUniformBuffer.u_View, pos);
    pos = mul(GlobalUniformBuffer.u_Projection, pos);
    output.Position = pos;

    return output;
}

float4 PS(VSOutput input) : SV_TARGET
{
	volatile float4 tex = texture0.Sample(sampler0, input.TexCoords);
    
	// return float4(input.TexCoords, 0, 1);
	return tex;
}
