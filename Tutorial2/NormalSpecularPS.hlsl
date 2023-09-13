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

float4 main(float4 viewPosition : Position, float4 normal : Normal, float4 tangent : Tangent, float4 bitangent : Bitangent, float2 texcoord : Texcoord) : SV_Target
{
    texcoord.y = 1.0f - texcoord.y;
    
    if (normalMapEnabled)
    {
        float3x3 tanToView = float3x3(normalize(tangent.xyz), normalize(bitangent.xyz), normalize(normal.xyz));
    
        const float3 normalSample = normalmap.Sample(samplr, texcoord).xyz;
        normal.xyz = normalSample * 2.0f - 1.0f;
    
        normal = float4(mul(normal.xyz, tanToView), 0.0f);
    }
    
    // light vector data
    float distance = length(lightPosition - viewPosition);
    float4 L = normalize(lightPosition - viewPosition);
    float4 N = normalize(normal);
    float4 V = normalize(-viewPosition);
    float4 R = reflect(-L, N);
    
    float4 ambient = ambientColor * ambientIntensity;
    
    // diffuse attenuation
    float attenuation = 1.0f / (attenuationConstant + attenuationLinear * distance + attenuationQuadradic * pow(distance, 2));
    
    // diffuse intensity
    float4 diffuse = diffuseColor * diffuseIntensity * attenuation * max(0, dot(L, N));

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
    
    float4 specular = attenuation * (diffuseColor * diffuseIntensity) * pow(max(0, dot(V, R)), power);
    float4 color = saturate((diffuse + ambient) * float4(tex.Sample(samplr, texcoord).rgb, 1.0f) + specular * specularReflection);
    color.a = 1;
    return color;
}