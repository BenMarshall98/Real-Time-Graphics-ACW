cbuffer inkBuffer : register(b7)
{
    matrix inkWorld;
    matrix inkView;
    matrix inkProjection;
    float inkHeight;
}

Texture2D inkTexture : register(t0);
SamplerState inkSampler : register(s0);

struct VS_OUTPUT
{
    float4 Pos : SV_POSITION;
    float3 FragPos : POSITION0;
    float3 InkCenter : POSITION1;
    float3 InkXDir : POSITION2;
    float3 InkZDir : POSITION3;
};

float4 main(VS_OUTPUT input) : SV_Target
{
    float3 fragInk = input.FragPos - input.InkCenter;
    
    float xDot = dot(fragInk, input.InkXDir);
    float zDot = dot(fragInk, input.InkZDir);
    
    xDot = xDot * 0.5f + 0.5f;
    zDot = zDot * 0.5f + 0.5f;
    
    //TODO: Finish
    
    return float4(xDot, zDot, 0.0f, 1.0f);
}