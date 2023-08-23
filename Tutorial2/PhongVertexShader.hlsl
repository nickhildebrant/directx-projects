cbuffer ConstantBuffer {
    matrix modelView;
    matrix worldViewProjection;
}

struct VertexShaderOutput {
    float4 worldPosition : Position;
    float4 normal : Normal;
    float2 texCoord : Texcoord;
    float4 position : SV_Position;
};

VertexShaderOutput main( float4 pos : POSITION, float4 n : Normal, float2 texcoord : Texcoord )
{
    VertexShaderOutput output;
    output.worldPosition = mul(pos, modelView);
    output.normal = mul(n, modelView);
    output.position = mul(pos, worldViewProjection);
    output.texCoord = texcoord;
    return output;
}