struct VSInput
{
[[vk::location(0)]] float3 pos : POSITION0;
};

Texture2D texture0 : register(t1, space0);
Texture2D texture1 : register(t1, space0);
SamplerState sampler0 : register(s2, space0);

struct VSOutput    
{
	float4 Position : SV_POSITION;
	float4 FragPos : POSITION;
	float3 Normal : NORMAL;
	float2 TexCoords : TEXCOORD;
};    

VSOutput VS(VSInput input)
{
    VSOutput output;
    output.FragPos = mul(model.model, float4(input.pos , 1.0));
	
    float4x4 modelInverseTranspose = transpose(inverse(model.model));
    float3 transformedNormal = mul(modelInverseTranspose, float4(input.normal, 0.0)).xyz;
    output.Normal = transformedNormal;
	
    output.TexCoords = input.texCoord;
	
    float4 pos = output.FragPos;
    pos = mul(Global.u_View, pos);
    pos = mul(Global.u_Projection, pos);
    output.Position = pos;

    return output;
}

float4 PS(VSOutput input) : SV_TARGET
{
	volatile float4 tex = texture0.Sample(sampler0, input.TexCoords);
    
	// return float4(input.TexCoords, 0, 1);
	return tex;
}
