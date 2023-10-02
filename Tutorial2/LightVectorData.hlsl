struct LightVectorData
{
    float4 L;
    float4 N;
    float4 V;
    float4 R;
};

LightVectorData CalculateLightData(float4 lightPosition, float4 viewPosition, float4 normal)
{
    // light vector data
    LightVectorData lightData;
    lightData.L = normalize(lightPosition - viewPosition);
    lightData.N = normalize(normal);
    lightData.V = normalize(-viewPosition);
    lightData.R = reflect(-lightData.L, lightData.N);
    
    return lightData;
}