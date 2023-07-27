cbuffer ConstantBuffer {
	matrix transform;
};

struct VertexShaderOutput {
	float4 color : Color;
	float4 position : SV_Position;
};

VertexShaderOutput main(float4 position : Position, float4 color : Color)
{
	VertexShaderOutput vertexOutput;
	vertexOutput.position = mul(position, transform);
	vertexOutput.color = color;
	return vertexOutput;
}