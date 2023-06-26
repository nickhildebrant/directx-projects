struct VertexShaderOutput {
	float3 color : Color;
	float4 position : SV_Position;
};

cbuffer ConstantBuffer {
	matrix transform;
};

VertexShaderOutput main(float2 pos : Position, float3 color : Color)
{
	VertexShaderOutput output;
	output.position = mul(float4(pos.x, pos.y, 0.0f, 1.0f), transform);
	output.color = color;

	return output;
}