struct GlobalUniformBufferStruct
{
    float4x4 u_View;
    float4x4 u_Projection;

    float3 u_CameraPos;
};

// [[vk::binding(binding-index, set-index)]]
// <type> <name>

[[vk::binding(0, 0)]]
ConstantBuffer<GlobalUniformBufferStruct> Global;