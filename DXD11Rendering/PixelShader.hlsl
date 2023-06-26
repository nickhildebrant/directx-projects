cbuffer ConstantBuffer {
	float4 faceColors[6];
};

float4 main(uint triangleID : SV_PrimitiveID) : SV_Target
{
	return faceColors[triangleID / 2];
}