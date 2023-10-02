float4 MapNormal(float4 normal, float4 tangent, float4 bitangent, float2 texcoord, Texture2D normalMap, SamplerState samplr)
{
    float3x3 tanToView = float3x3(normalize(tangent.xyz), normalize(bitangent.xyz), normalize(normal.xyz));
    
    float3 normalSample = normalMap.Sample(samplr, texcoord).xyz;
    normal.xyz = normalSample * 2.0f - 1.0f;
    
    return float4(mul(normal.xyz, tanToView), 0.0f);
}

float Attenuate(float attConstant, float attLinear, float attQuadradic, float distance)
{
    return 1.0f / (attConstant + attLinear * distance + attQuadradic * pow(distance, 2));
}

float4 DiffuseCalculation(float4 color, float intensity, float att, float4 LightVector, float4 NormalVector)
{
    return color * intensity * att * max(0, dot(LightVector, NormalVector));
}

float4 SpecularCalculation(float4 color, float intensity, float att, float power, float4 ViewVector, float4 ReflectionVector)
{
    return att * (color * intensity) * pow(max(0, dot(ViewVector, ReflectionVector)), power);
}