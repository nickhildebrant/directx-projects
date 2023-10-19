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

cbuffer PSMaterialConstant
{
    float specularIntensity;
    float shininess;
    bool normalMapEnabled;
    float padding;
};

Texture2D tex;
Texture2D normalmap : register(t2);
SamplerState samplr;

static const float4 specularColor = { 1.0f, 1.0f, 1.0f, 1.0f };

static const float ambientIntensity = 1.0f;

float4 main(float4 viewPosition : Position, float4 normal : Normal, float4 tangent : Tangent, float4 bitangent : Bitangent, float2 texcoord : Texcoord) : SV_Target
{ 
    //texcoord.y = 1.0f - texcoord.y;
    
    if (normalMapEnabled)
    {
        float3x3 tanToView = float3x3(normalize(tangent.xyz), normalize(bitangent.xyz), normalize(normal.xyz));
    
        float3 normalSample = normalmap.Sample(samplr, texcoord).xyz;
        normal.xyz = normalSample * 2.0f - 1.0f;
        normal.w = 0.0f;
    
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

    float4 specular = attenuation * pow(max(0, dot(V, R)), shininess) * specularColor * specularIntensity;
    float4 color = saturate(ambient + diffuse) * float4(tex.Sample(samplr, texcoord).rgb, 1.0f) + specular.a;
    color.a = 1;
    return color;
}