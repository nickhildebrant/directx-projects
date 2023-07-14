cbuffer ConstantBuffer {
	matrix transform;
};

struct VertexShaderOutput {
	float4 color : Color;
	float4 position : SV_Position;
};

VertexShaderOutput main( float3 position : Position, float4 color : Color )
{
	VertexShaderOutput vertexOutput;
	vertexOutput.position = mul( float4( position, 1.0f ), transform );
	vertexOutput.color = color;
	return vertexOutput;
}