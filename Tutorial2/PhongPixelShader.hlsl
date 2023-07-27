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
};

static const float4 specularColor = { 1.0f, 1.0f, 1.0f, 1.0f };

static const float4 cameraPosition = { 0.0f, 0.0f, 20.0f, 1.0f };

static const float ambientIntensity = 1.0f;
static const float lightIntensity = 1.0f;
static const float specularIntensity = 1.0f;

static const float shininess = 2.0f;

float4 main( float4 worldPosition : Position, float4 normal : Normal) : SV_Target
{
    // light vector data
    const float distance = length(lightPosition - worldPosition);
    const float4 L = normalize(lightPosition - worldPosition);
    const float4 N = normalize(normal);
    const float4 V = normalize(cameraPosition - worldPosition);
    const float4 R = reflect(-L, N);
    
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