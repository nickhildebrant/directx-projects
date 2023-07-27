cbuffer ConstantBuffer {
    matrix world;
    matrix worldViewProjection;
}

struct VertexShaderOutput {
    float4 worldPosition : Position;
    float4 normal : Normal;
    float4 position : SV_Position;
};

VertexShaderOutput main( float4 pos : POSITION, float4 n : Normal )
{
    VertexShaderOutput output;
    output.worldPosition = mul(pos, world);
    output.normal = mul(n, world);
    output.position = mul(pos, worldViewProjection);
    return output;
}