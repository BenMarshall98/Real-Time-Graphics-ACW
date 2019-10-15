cbuffer cameraBuffer : register(b0)
{
	//matrix World;
	matrix View;
	matrix Projection;
}

cbuffer modelBuffer : register(b1)
{
	matrix World;
	matrix InverseWorld;
	float4 Color;
}

//--------------------------------------------------------------------------------------
struct VS_OUTPUT
{
	float4 Pos : SV_POSITION;
	float4 Col : COLOR0;
	float4 Normal : NORMAL0;
};

VS_OUTPUT main(float4 Pos : POSITION, float4 Normal : NORMAL)
{
	VS_OUTPUT output = (VS_OUTPUT)0;
	output.Pos = mul(Pos, World);
	output.Pos = mul(output.Pos, View);
	output.Pos = mul(output.Pos, Projection);
	output.Normal = mul(Normal, InverseWorld);
	output.Col = Color;
	return output;
}
