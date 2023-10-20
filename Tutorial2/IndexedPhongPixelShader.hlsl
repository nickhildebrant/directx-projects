#include "ShaderCalculations.hlsl"
#include "PointLight.hlsl"
#include "LightVectorData.hlsl"

cbuffer ObjectConstantBuffer
{
    float4 materialColors[6];
    float specularIntensity;
    float shininess;
};

static const float4 specularColor = { 1.0f, 1.0f, 1.0f, 1.0f };

static const float ambientIntensity = 1.0f;
static const float lightIntensity = 1.0f;

float4 main(float4 worldPosition : Position, float4 normal : Normal, uint tid : SV_PrimitiveID) : SV_Target
{
    float distance = length(lightPosition - worldPosition);
    
    // light vector data
    LightVectorData lightData = CalculateLightData(lightPosition, worldPosition, normal);
    
    float4 ambient = ambientColor * ambientIntensity;
    float attenuation = Attenuate(attenuationConstant, attenuationLinear, attenuationQuadradic, distance);
    float4 diffuse = DiffuseCalculation(diffuseColor, diffuseIntensity, attenuation, lightData.L, lightData.N);
    float4 specular = SpecularCalculation(diffuseColor, diffuseIntensity, attenuation, shininess, lightData.V, lightData.R);
    float4 color = saturate(ambient + diffuse + specular) * materialColors[(tid / 2) % 6];

    color.a = 1;
    return color;
}