
#include "../CommonFunction.hlsli"
#include "../CommonStruct.hlsli"
  
DefaultVSOutput VS(DefaultVSInput input)
{
    return CommonVS(input);
}

float4 PS(DefaultVSOutput input) : SV_TARGET
{
    return float4(1.0, 1.0, 1.0, 1.0);
}
