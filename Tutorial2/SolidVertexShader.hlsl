cbuffer ConstantBuffer 
{
    matrix world;
    matrix worldViewProjection;
};

float4 main( float4 position : POSITION ) : SV_POSITION
{    
    return mul(position, worldViewProjection);
}