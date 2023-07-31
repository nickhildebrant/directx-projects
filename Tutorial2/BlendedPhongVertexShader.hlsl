cbuffer ConstantBuffer
{
    matrix modelView;
    matrix modelViewProj;
};

struct VertexShaderOutput
{
    float4 worldPos : Position;
    float4 normal : Normal;
    float4 color : Color;
    float4 pos : SV_Position;
};

VertexShaderOutput main(float4 pos : Position, float4 n : Normal, float4 color : Color)
{
    VertexShaderOutput vso;
    vso.worldPos = mul(pos, modelView);
    vso.normal = mul(n, modelView);
    vso.pos = mul(pos, modelViewProj);
    vso.color = color;
    return vso;
}