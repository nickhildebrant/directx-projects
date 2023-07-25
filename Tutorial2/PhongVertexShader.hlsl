cbuffer ConstantBuffer {
    matrix world;
    matrix worldViewProjection;
}

struct VertexShaderOutput {
    float3 worldPosition : Position;
    float3 normal : Normal;
    float4 position : SV_Position;
};

VertexShaderOutput main( float4 pos : POSITION, float3 n : Normal )
{
    VertexShaderOutput output;
    output.worldPosition = (float3) mul(float4(pos, 1.0f), world);
    output.normal = mul(n, (float3x3) world);
    output.position = mul(float4(pos, 1.0f), worldViewProjection);
    return output;
}