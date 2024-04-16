#include "./UboTest.hlsli"

struct VSInput
{
[[vk::location(0)]] float3 pos : POSITION0;
[[vk::location(1)]] float3 normal : COLOR0;
[[vk::location(2)]] float2 texCoord : TEXCOORD;
};

struct UBO
{
	float4x4 model;
	float4x4 u_View;
	float4x4 u_Projection;
};

cbuffer ubo : register(b0, space0) { UBO ubo; }

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
    output.FragPos = mul(float4(input.pos , 1.0), ubo.model);
	float3 transformedNormal = mul(float4(input.normal, 0.0), ubo.model).xyz;
    output.Normal = transformedNormal;
    output.TexCoords = input.texCoord;
    float4 pos = output.FragPos;
	pos = mul(mul(ubo.u_Projection, ubo.u_View), pos);
	output.Position = pos;
	
    return output;
}

float4 PS(VSOutput input) : SV_TARGET
{
	
	return float4(input.Normal, 1.0);
}