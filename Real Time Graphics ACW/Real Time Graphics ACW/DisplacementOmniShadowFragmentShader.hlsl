cbuffer ShadowLightBuffer : register(b5)
{
    float3 LightPosition;
    float FarPlane;
};

struct GS_OUTPUT
{
    float4 Pos : SV_Position;
    float3 FragPos : POSITION0;
    uint Layer : SV_RenderTargetArrayIndex;
};

float main(GS_OUTPUT input) : SV_Depth
{
    float distance = length(input.FragPos.xyz - LightPosition.xyz);
    distance = distance / FarPlane;
    return distance;
}