cbuffer ConstantBuffer
{
    matrix modelView;
    matrix modelViewProj;
};

struct VertexShaderOutput {
    float4 viewPosition : Position;
    float4 normal : Normal;
    float2 texCoord : Texcoord;
    float4 position : SV_Position;
};

VertexShaderOutput main(float4 pos : Position, float4 n : Normal, float2 tc : Texcoord)
{
    VertexShaderOutput vso;
    vso.viewPosition = mul(pos, modelView);
    vso.normal = mul(n, modelView);
    vso.position = mul(pos, modelViewProj);
    vso.texCoord = tc;
    return vso;
}