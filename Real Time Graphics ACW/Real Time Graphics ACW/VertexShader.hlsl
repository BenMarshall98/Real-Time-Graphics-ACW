cbuffer cameraBuffer : register(b0)
{
	matrix View;
	matrix Projection;
}

cbuffer modelBuffer : register(b1)
{
	matrix World;
	matrix InverseWorld;
}

//--------------------------------------------------------------------------------------
struct VS_OUTPUT
{
	float4 Pos : SV_POSITION;
	float4 FragmentPos : POSITION0;
	float4 Normal : NORMAL0;
};

VS_OUTPUT main(float4 Pos : POSITION, float4 Normal : NORMAL)
{
	VS_OUTPUT output = (VS_OUTPUT)0;
	output.Pos = mul(Pos, World);
	output.Pos = mul(output.Pos, View);
	output.Pos = mul(output.Pos, Projection);
	output.FragmentPos = mul(Pos, World);
	output.Normal = mul(Normal, InverseWorld);
	return output;
}
