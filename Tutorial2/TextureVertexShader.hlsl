cbuffer CBuf {
	matrix transform;
};

struct VertexShaderOutput {
	float2 tex : TexCoord;
	float4 pos : SV_Position;
};

VertexShaderOutput main( float4 position : Position, float2 texCord : TexCoord )
{
	VertexShaderOutput output;
	output.pos = mul( position, transform );
	output.tex = texCord;
	return output;
}