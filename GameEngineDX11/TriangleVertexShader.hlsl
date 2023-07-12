struct Input {
	float2 position : POSITION;
	float3 color : COLOR;
};

struct Output {
	float4 position : SV_POSITION;
	float3 color : COLOR;
};

cbuffer ColorConstantBuffer : register( b0 ) {
	float RedLevel;
	float GreenLevel;
	float BlueLevel;
};

cbuffer PositionConstantBuffer : register( b1 ) {
	float4 posMatrix;
};

Output main( Input input )
{
	Output output;

	output.position = float4( input.position.x, input.position.y, 0, 1 );
	output.position.x += posMatrix.x;
	output.position.x += posMatrix.y;
	output.position.xy *= posMatrix.z;

	output.color = input.color;
	output.color.r *= RedLevel;
	output.color.g *= GreenLevel;
	output.color.b *= BlueLevel;

	return output;
}