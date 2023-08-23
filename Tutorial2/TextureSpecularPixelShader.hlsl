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

Texture2D tex;
Texture2D specular;
SamplerState samplr;

static const float ambientIntensity = 1.0f;
static const float lightIntensity = 1.0f;

float4 main(float4 worldPosition : Position, float4 normal : Normal, float2 texcoord : Texcoord) : SV_Target
{
    // light vector data
    float distance = length(lightPosition - worldPosition);
    float4 L = normalize(lightPosition - worldPosition);
    float4 N = normalize(normal);
    float4 V = normalize(-worldPosition);
    float4 R = reflect(-L, N);
    
    float4 ambient = ambientColor * ambientIntensity;
    
    // diffuse attenuation
    float attenuation = 1.0f / (attenuationConstant + attenuationLinear * distance + attenuationQuadradic * pow(distance, 2));
    
    // diffuse intensity
    float4 diffuse = diffuseColor * diffuseIntensity * attenuation * max(0, dot(L, N));

    float4 specularSample = specular.Sample(samplr, texcoord);
    float4 specularIntensity = specularSample.rgba;
    float4 specular = attenuation * pow(max(0, dot(V, R)), specularIntensity.a) * float4(specularIntensity.rgb, 1.0f);
    
    float4 color = saturate(diffuse + ambient + specular) * tex.Sample(samplr, texcoord);
    color.a = 1;
    return color;
}