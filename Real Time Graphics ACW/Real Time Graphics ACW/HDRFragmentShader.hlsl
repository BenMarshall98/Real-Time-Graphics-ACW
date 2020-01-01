cbuffer GlobalBuffer : register(b10)
{
    float Time;
    int ScreenMode;
    int ShadowMode;
    float InkHeight;
}

Texture2D normalTexture : register(t6);
SamplerState normalSampler : register(s6);

Texture2D bloomTexture : register(t7);
SamplerState bloomSampler : register(s7);

struct VS_OUTPUT
{
    float4 Pos : SV_POSITION;
    float2 TexCoord : TEXCOORD0;
};

//TODO: http://www.cs.utah.edu/~reinhard/cdrom/tonemap.pdf

float4 main(VS_OUTPUT input) : SV_Target
{
    float3 color = normalTexture.Sample(normalSampler, input.TexCoord).rgb;
    
    if (ScreenMode > 7) //HDR or Bloom else deferred
    {
        color = color / (color + float3(1.0f, 1.0f, 1.0f));
    }
    
    return float4(color, 1.0f);
}