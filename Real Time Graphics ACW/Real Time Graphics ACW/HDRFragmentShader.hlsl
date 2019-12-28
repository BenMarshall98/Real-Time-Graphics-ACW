Texture2D normalTexture : register(t0);
SamplerState normalSampler : register(s0);

Texture2D bloomTexture : register(t1);
SamplerState bloomSampler : register(s1);

struct VS_OUTPUT
{
    float4 Pos : SV_POSITION;
    float2 TexCoord : TEXCOORD0;
};

//TODO: http://www.cs.utah.edu/~reinhard/cdrom/tonemap.pdf

float4 main(VS_OUTPUT input) : SV_Target
{
    float3 color = normalTexture.Sample(normalSampler, input.TexCoord).rgb;
    
    color = color / (color + float3(1.0f, 1.0f, 1.0f));
    
    return float4(color, 1.0f);
}