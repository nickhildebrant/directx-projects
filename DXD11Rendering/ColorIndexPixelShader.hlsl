cbuffer ConstantBuffer {
	float4 face_colors[8];
};

float4 main(uint triangleID : SV_PrimitiveID) : SV_Target
{
	return faceColors[(triangleID / 2) % 8];
}