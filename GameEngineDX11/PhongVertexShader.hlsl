cbuffer WorldlViewProjection : register( b0 ) {
	matrix World;
	matrix View;
	matrix Projection;
};

cbuffer LightConstantBuffer : register( b1 ) {
	float4 lightPostion;
};

struct VertexShaderInput {
	float4 Position : POSITION;
	float4 Normal : NORMAL;
};

struct PixelShaderInput {
	float4 Position : SV_POSITION;
	float4 Color : COLOR;
	float4 Normal : TEXCOORD0;
	float4 WorldPosition : TEXCOORD1;
};

PixelShaderInput main( VertexShaderInput input )
{
	PixelShaderInput output;

	float4 worldPosition = mul( input.Position, World );
	float4 viewPosition = mul( worldPosition, View );
	output.Position = mul( viewPosition, Projection );
	output.WorldPosition = worldPosition;
	output.Normal = input.Normal;
	output.Color = 0;

	return output;
}