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
SamplerState textureSampler;

static const float ambientIntensity = 1.0f;

float4 main(float4 viewPosition : Position, float4 normal : Normal, float2 texCoord : Texcoord) : SV_Target
{
    texCoord.y = 1.0f - texCoord.y;
    
    // light vector data
    float distance = length(lightPosition - viewPosition);
    LightVectorData lightData = CalculateLightData(lightPosition, viewPosition, normal);
    
    float4 ambient = ambientColor * ambientIntensity;
    float attenuation = Attenuate(attenuationConstant, attenuationLinear, attenuationQuadradic, distance);
    float4 diffuse = DiffuseCalculation(diffuseColor, diffuseIntensity, attenuation, lightData.L, lightData.N);
    float4 specular = SpecularCalculation(diffuseColor, diffuseIntensity, attenuation, shininess, lightData.V, lightData.R);
    float4 color = saturate(ambient + diffuse + specular) * tex.Sample(textureSampler, texCoord);

    color.a = 1;
    return color;
}