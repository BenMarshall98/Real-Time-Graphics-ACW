cbuffer cameraBuffer : register(b0)
{
	float3 viewPosition;
}

cbuffer directionalBuffer : register(b2)
{
	float4 DirectionalColor;
	float3 DirectionalDirection;
	int DirectionalUsed;
}

struct VS_OUTPUT
{
	float4 Pos : SV_POSITION;
	float4 FragmentPos : POSITION0;
	float4 Normal : NORMAL0;
};

float4 main(VS_OUTPUT input) : SV_Target
{
	float4 color = float4(0.0f, 0.0f, 0.0f, 0.0f);

	float3 L = normalize(DirectionalDirection);
	float3 N = normalize(input.Normal.xyz);
	return max(0.0f, dot(L, N)) * DirectionalColor;
	//return input.Col;
}