cbuffer LightConstantBuffer {
    float4 lightPosition;
    
    float4 materialColor;
    float4 ambientColor;
    float4 diffuseColor;
    
    float diffuseIntensity;
    float attenuationConstant;
    float attenuationLinear;
    float attenuationQuadradic;
};

static const float4 specularColor = { 1.0f, 1.0f, 1.0f, 1.0f };

static const float3 cameraPosition = { 0.0f, 0.0f, 20.0f };

static const float ambientIntensity = 1.0f;
static const float lightIntensity = 1.0f;
static const float specularIntensity = 1.0f;

static const float shininess = 2.0f;

float4 main( float3 worldPosition : Position, float3 normal : Normal) : SV_Target
{
    // light vector data
    const float distance = length(lightPosition - worldPosition);
    const float3 L = normalize(lightPosition - worldPosition);
    const float3 N = normalize(normal);
    const float3 V = normalize(cameraPosition - worldPosition);
    const float3 R = reflect(-L, N);
    
    // diffuse attenuation
    const float att = 1.0f / (attenuationConstant + attenuationLinear * distance + attenuationQuadradic * pow(distance, 2));
    
    // diffuse intensity
    const float4 diffuse = diffuseColor * diffuseIntensity * att * max(0.0f, dot(L, N));
    
    float4 ambient = ambientColor * ambientIntensity;
    //float4 diffuse = diffuseIntensity * diffuseColor * max(0, dot(N, L));
    //float4 specular = pow(max(0, dot(V, R)), shininess) * specularColor * specularIntensity;
    //float4 color = saturate(ambient + diffuse + specular);
    
    return saturate(diffuse + ambient);
   // +specular);
    //color.a = 1;
    //return color;
}