#include "ShaderCalculations.hlsl"
#include "PointLight.hlsl"
#include "LightVectorData.hlsl"

cbuffer ObjectConstantBuffer
{
    float specularIntensity;
    float shininess;
    float padding[2];
};

Texture2D tex;
SamplerState samplr;

static const float ambientIntensity = 1.0f;

float4 main( float4 viewPosition : Position, float4 normal : Normal, float2 texcoord : Texcoord ) : SV_Target
{
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