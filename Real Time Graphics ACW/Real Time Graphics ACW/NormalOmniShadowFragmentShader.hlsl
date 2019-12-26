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
    
float4 main(GS_OUTPUT input) : SV_Target
{
    float distance = length(input.FragPos.xyz - LightPosition.xyz);
    distance = distance / FarPlane;
    
    float moment1 = distance;
    float moment2 = distance * distance;
    
    float dx = ddx(distance);
    float dy = ddy(distance);
    
    moment2 += 0.25f * (dx * dx + dy * dy);
    
    return float4(moment1, moment2, 0.0f, 1.0f);
}

