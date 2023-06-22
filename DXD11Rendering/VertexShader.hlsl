struct VertexShaderOutput {
	float3 color : Color;
	float4 position : SV_Position;
};

VertexShaderOutput main(float2 pos : Position, float3 color : Color)
{
	VertexShaderOutput output;
	output.position = float4(pos.x, pos.y, 0.0f, 1.0f);
	output.color = color;

	return output;
}