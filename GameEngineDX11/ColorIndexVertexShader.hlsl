cbuffer ConstantBuffer {
	matrix transform;
};

float4 main( float3 position : Position ) : SV_POSITION
{
	return mul( float4( position, 1.0f ), transform );
}