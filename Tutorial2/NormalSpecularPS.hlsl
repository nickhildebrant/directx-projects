cbuffer LightConstantBuffer
{
    float4 lightPosition;
    
    float4 ambientColor;
    float4 diffuseColor;
    
    float diffuseIntensity;
    float attenuationConstant;
    float attenuationLinear;
    float attenuationQuadradic;
};

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