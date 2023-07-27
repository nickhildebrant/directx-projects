cbuffer LightConstantBuffer {
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
    float4 materialColor;
    float specularIntensity;
    float shininess;
};

static const float4 specularColor = { 1.0f, 1.0f, 1.0f, 1.0f };

static const float4 cameraPosition = { 0.0f, 0.0f, 20.0f, 1.0f };

static const float ambientIntensity = 1.0f;
static const float lightIntensity = 1.0f;

float4 main( float4 worldPosition : Position, float4 normal : Normal) : SV_Target
{
    // light vector data
    float distance = length(lightPosition - worldPosition);
    float4 L = normalize(lightPosition - worldPosition);
    float4 N = normalize(normal);
    float4 V = normalize(cameraPosition - worldPosition);
    float4 R = reflect(-L, N);
    
    float4 ambient = ambientColor * ambientIntensity;
    
    // diffuse attenuation
    float attenuation = 1.0f / (attenuationConstant + attenuationLinear * distance + attenuationQuadradic * pow(distance, 2));
    
    // diffuse intensity
    float4 diffuse = diffuseColor * diffuseIntensity * attenuation * max(0.0f, dot(L, N));

    float4 specular = attenuation * pow(max(0, dot(V, R)), shininess) * specularColor * specularIntensity;
    float4 color = saturate(ambient + diffuse + specular) * materialColor;

    color.a = 1;
    return color;
}