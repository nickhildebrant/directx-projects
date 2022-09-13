struct Input {
	float2 position : POSITION;
	float3 color : COLOR;
};

struct Output {
	float4 position : SV_POSITION;
	float3 color : COLOR;
};

cbuffer ColorConstantBuffer {
	float RedLevel;
	float GreenLevel;
	float BlueLevel;
};

cbuffer PositionConstantBuffer {
	float xOffset;
	float yOffset;
	float zOffset;
};

Output main(Input input)
{
	Output output;

	output.position = float4(input.position.x, input.position.y, 0, 1);
	//output.position.x += xOffset;
	//output.position.x += yOffset;
	//output.position.xy *= zOffset;

	output.color = input.color;
	output.color.r *= RedLevel;
	output.color.g *= GreenLevel;
	output.color.b *= BlueLevel;

	return output;
}