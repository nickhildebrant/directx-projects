cbuffer ConstantBuffer
{
    matrix view;
    matrix projection;
};

struct VertexShaderOut {
    float4 viewPosition : Position;
    float4 normal : Normal;
    float4 position : SV_Position;
};

VertexShaderOut main(float4 position : POSITION, float4 normal : NORMAL)
{
    VertexShaderOut vso;
    vso.viewPosition = mul(position, view);
    vso.normal = mul(normal, view);
    vso.position = mul(position, projection);
    return vso;
}