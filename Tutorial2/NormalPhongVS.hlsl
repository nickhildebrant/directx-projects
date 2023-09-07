cbuffer ConstantBuffer
{
    matrix view;
    matrix projection;
}

struct VertexShaderOuput
{
    float4 viewPosition : Position;
    float4 normal : Normal;
    float4 tan : Tangent;
    float4 bitan : Bitangent;
    float2 texCoord : Texcoord;
    float4 position : SV_Position;
};

VertexShaderOuput main(float4 pos : Position, float4 n : Normal, float4 t : Tangent, float4 b : Bitangent, float2 tc : Texcoord)
{
    VertexShaderOuput vso;
    vso.viewPosition = mul(pos, view);
    vso.normal = mul(n, view);
    vso.tan = mul(t, view);
    vso.bitan = mul(b, view);
    vso.position = mul(pos, projection);
    vso.texCoord = tc;
    
    return vso;
}