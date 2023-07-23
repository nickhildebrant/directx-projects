Texture2D text;

SamplerState smplr;

float4 main( float2 tc : TexCoord ) : SV_Target
{
	return tex.Sample( smplr,tc );
}