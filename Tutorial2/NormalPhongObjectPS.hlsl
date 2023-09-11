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
    
        normal = mul(normal, world);
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