struct VS_OUTPUT
{
	float4 Pos : SV_POSITION;
	float4 Col : COLOR0;
	float4 Normal : NORMAL0;
};

float4 main(VS_OUTPUT input) : SV_Target
{
	float3 L = normalize(float3(0, 0, -1));
	float3 N = normalize(input.Normal.xyz);
	return max(0.0f, dot(L, N)) * float4(1.0f, 1.0f, 1.0f, 1.0f) * input.Col;
	//return input.Col;
}