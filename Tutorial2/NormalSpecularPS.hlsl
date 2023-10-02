#include "ShaderCalculations.hlsl"
#include "PointLight.hlsl"

cbuffer ObjectConstantBuffer
{
    float4 specularColor;
    float specularWeight;
    float specularPower;
    bool specularMapEnabled;
    
    bool normalMapEnabled;
    bool hasGloss;
    float padding[3];
};

Texture2D tex;
Texture2D specular;
Texture2D normalmap;
SamplerState samplr;

static const float ambientIntensity = 1.0f;
static const float lightIntensity = 1.0f;

float4 main(float4 viewPosition : Position, float4 normal : Normal, float4 tangent : Tangent, float4 bitangent : Bitangent, float2 texcoord : Texcoord) : SV_Target
{
    texcoord.y = 1.0f - texcoord.y;
    
    if (normalMapEnabled)
    {
        normal = normalize(MapNormal(normal, tangent, bitangent, texcoord, normalmap, samplr));
    }
    
    // light vector data
    float distance = length(lightPosition - viewPosition);
    float4 L = normalize(lightPosition - viewPosition);
    float4 N = normalize(normal);
    float4 V = normalize(-viewPosition);
    float4 R = reflect(-L, N);

    float4 specularReflection;
    float power = specularPower;
    if(specularMapEnabled)
    {
        float4 specularSample = specular.Sample(samplr, texcoord);
        specularReflection = float4(specularSample.rgb * specularWeight, 1.0f);
        
        if(hasGloss)
        {
            power = pow(2.0f, specularSample.a * 13.0f);
        }
    }
    else
    {
        specularReflection = specularColor;
    }
    
    float4 ambient = ambientColor * ambientIntensity;
    float attenuation = Attenuate(attenuationConstant, attenuationLinear, attenuationQuadradic, distance);
    float4 diffuse = DiffuseCalculation(diffuseColor, diffuseIntensity, attenuation, L, N);
    float4 specular = SpecularCalculation(diffuseColor, diffuseIntensity, attenuation, power, V, R);
    float4 color = saturate((diffuse + ambient) * float4(tex.Sample(samplr, texcoord).rgb, 1.0f) + specular * specularReflection);
    color.a = 1;
    return color;
}