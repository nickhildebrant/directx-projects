#include "ShaderCalculations.hlsl"
#include "LightVectorData.hlsl"
#include "PointLight.hlsl"

cbuffer PSMaterialConstant
{
    float specularPower;
    bool hasGloss;
    float specularWeight;
    float padding;
};

Texture2D diffuseTexture;
Texture2D specularTexture;
SamplerState smplr;

const float ambientIntensity = 1.0f;

float4 main(float4 viewPosition : Position, float4 normal : Normal, float2 texcoord : Texcoord) : SV_Target
{
    // light vector data
    normal = normalize(normal);
    float distance = length(lightPosition - viewPosition);
    LightVectorData lightData = CalculateLightData(lightPosition, viewPosition, normal);
    
    float4 specularSample = specularTexture.Sample(smplr, texcoord);
    float4 specularReflectionColor = specularSample * specularWeight;
    specularReflectionColor.a = 1.0f;

    float shininess = specularPower;
    if (hasGloss)
    {
        shininess = pow(2.0f, specularSample.a * 13.0f);
    }

    float4 ambient = ambientColor * ambientIntensity;
    float attenuation = Attenuate(attenuationConstant, attenuationLinear, attenuationQuadradic, distance);
    float4 diffuse = DiffuseCalculation(diffuseColor, diffuseIntensity, attenuation, lightData.L, lightData.N);
    float4 specular = SpecularCalculation(diffuseColor, diffuseIntensity, attenuation, shininess, lightData.V, lightData.R);
    float4 color = saturate(ambient + diffuse) * diffuseTexture.Sample(smplr, texcoord) + specular;
    color.a = 1;
    return color;
}
