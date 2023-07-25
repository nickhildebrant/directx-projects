cbuffer LightConstantBuffer {
    float3 lightPosition;
};

static const float3 materialColor = { 0.7f, 0.7f, 0.9f };
static const float3 ambientColor = { 0.05f, 0.05f, 0.05f };
static const float3 diffuseColor = { 0.7f, 0.7f, 0.9f };
static const float diffuseIntensity = 1.0f;
static const float lightIntensity = 1.0f;

static const float attConst = 0.045f;
static const float attQuad = 0.0075f;

float4 main( float3 worldPosition : Position, float3 normal : Normal) : SV_Target
{
    // light vector data
    const float3 L = lightPosition - worldPosition;
    const float distance = length(L);
    const float3 lightDirection = L / distance;
    
    // diffuse attenuation
    const float att = 1.0f / (attConst + lightIntensity * lightDirection + attQuad * (distance * distance));
    
    // diffuse intensity
    const float3 diffuse = diffuseColor * diffuseIntensity * att * max(0.0f, dot(lightDirection, normal));
    
    return float4(saturate(diffuse + ambientColor), 1.0f);
}