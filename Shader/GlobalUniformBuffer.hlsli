struct GlobalUniformBuffer
{
    float4x4 u_View;
    float4x4 u_Projection;
};

// [[vk::binding(binding-index, set-index)]]
// <type> <name>

[[vk::binding(0, 0)]]
ConstantBuffer<GlobalUniformBuffer> GlobalUniformBuffer;