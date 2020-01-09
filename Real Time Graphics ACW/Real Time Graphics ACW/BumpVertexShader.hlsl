cbuffer cameraBuffer : register(b0)
{
	matrix View;
	matrix Projection;
	float3 ViewPosition;
}

cbuffer modelBuffer : register(b1)
{
	matrix World;
	matrix InverseWorld;
}

cbuffer directionalBuffer : register(b3)
{
    float4 DirectionalColor;
    float3 DirectionalDirection;
    int DirectionalUsed;
    matrix DirectionalView;
    matrix DirectionalProjection;
}

struct VS_INPUT
{
    float3 Pos : POSITION;
    float3 Normal : NORMAL;
    float2 TexCoord : TEXCOORD;
    float3 Tangent : TANGENT;
    float3 BiTangent : BITANGENT;
};

struct VS_OUTPUT
{
	float4 Pos : SV_POSITION;
	float3 FragmentPos : POSITION0;
	float3 Normal : NORMAL0;
    float2 TexCoord : TEXCOORD0;
	float3 ViewPosition : POSITION1;
    float4 LightFragmentPos : POSITION2;
    float4 Position : POSITION3;
    float3x3 TBN : POSITION4;
};

VS_OUTPUT main(VS_INPUT input)
{
	VS_OUTPUT output = (VS_OUTPUT)0;
    output.Pos = mul(float4(input.Pos, 1.0f), World);
    output.FragmentPos = output.Pos.xyz;
    output.LightFragmentPos = mul(output.Pos, DirectionalView);
    output.LightFragmentPos = mul(output.LightFragmentPos, DirectionalProjection);
	output.Pos = mul(output.Pos, View);
	output.Pos = mul(output.Pos, Projection);
    output.Position = output.Pos;
    output.Normal = mul(float4(input.Normal, 1.0f), InverseWorld).xyz;
    float3 Tangent = mul(float4(input.Tangent, 1.0f), InverseWorld).xyz;
    float3 BiTangent = mul(float4(input.BiTangent, 1.0f), InverseWorld).xyz;
    output.TexCoord = input.TexCoord;
	output.ViewPosition = ViewPosition;
    
    output.TBN = float3x3(Tangent, BiTangent, output.Normal);
	return output;
}
