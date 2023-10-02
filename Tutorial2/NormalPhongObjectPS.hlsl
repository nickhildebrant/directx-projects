#include "ShaderCalculations.hlsl"
#include "PointLight.hlsl"
#include "LightVectorData.hlsl"

cbuffer PSMaterialConstant
{
    float specularIntensity;
    float shininess;
    bool normalMapEnabled;
    float padding;
};

cbuffer TransformConstantBuffer
{
    matrix world;
    matrix view;
};

Texture2D tex;
Texture2D normalmap : register(t2);
SamplerState samplr;

static const float4 specularColor = { 1.0f, 1.0f, 1.0f, 1.0f };

static const float ambientIntensity = 1.0f;

float4 main(float4 viewPosition : Position, float4 normal : Normal, float2 texcoord : Texcoord) : SV_Target
{
    if (normalMapEnabled)
    {
        float3 normalSample = normalmap.Sample(samplr, texcoord).xyz;
        normal.x = normalSample.x * 2.0f - 1.0f;
        normal.y = -normalSample.y * 2.0f + 1.0f;
        normal.z = normalSample.z * 2.0f - 1.0f;
        //normal.w = 0.0f;
    
        normal = normalize(mul(normal, world));
    }
    
    // light vector data
    float distance = length(lightPosition - viewPosition);
    LightVectorData lightData = CalculateLightData(lightPosition, viewPosition, normal);
    
    float4 ambient = ambientColor * ambientIntensity;
    float attenuation = Attenuate(attenuationConstant, attenuationLinear, attenuationQuadradic, distance);
    float4 diffuse = DiffuseCalculation(diffuseColor, diffuseIntensity, attenuation, lightData.L, lightData.N);
    float4 specular = SpecularCalculation(diffuseColor, diffuseIntensity, attenuation, shininess, lightData.V, lightData.R);
    float4 color = saturate(ambient + diffuse) * float4(tex.Sample(samplr, texcoord).rgb, 1.0f) + specular.a;
    color.a = 1;
    return color;
}