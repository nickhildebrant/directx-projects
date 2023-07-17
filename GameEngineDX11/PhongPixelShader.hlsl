cbuffer MaterialConstantBuffer : register( b2 ) {
	float4 lightColor;
	float4 Ka;
	float4 Kd;
	float4 Ks;
	float4 shininess;
};

struct PixelShaderInput {
	float4 Position : SV_POSITION;
	float3 OutVector : POSITION;
	float3 Normal : NORMAL0;
	float3 Light : POSITION1;
};

float4 main( PixelShaderInput input ) : SV_TARGET
{
	float3 L = normalize( input.Light );
	float3 V = normalize( input.OutVector );
	float3 R = normalize( reflect( L, input.Normal ) );

	float4 diffuse = Ka + ( lightColor * Kd * max( dot( input.Normal, L ), 0.0f ) );
	diffuse = saturate( diffuse );

	float4 specular = Ks * pow( max( dot( R, V ), 0.0f ), shininess.x - 50.0f );
	specular = saturate( specular );

	return diffuse + specular;
}