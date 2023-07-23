cbuffer CBuf {
	matrix transform;
};

struct VertexShaderOutput {
	float2 tex : TexCoord;
	float4 pos : SV_Position;
};

VertexShaderOutput main( float3 position : Position, float2 texCord : TexCoord )
{
	VertexShaderOutput output;
	output.pos = mul( float4( position, 1.0f ), transform );
	output.tex = texCord;
	return output;
}