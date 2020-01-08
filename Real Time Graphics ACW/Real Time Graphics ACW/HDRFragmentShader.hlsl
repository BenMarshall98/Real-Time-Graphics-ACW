cbuffer GlobalBuffer : register(b10)
{
    float Time;
    int ScreenMode;
    int ShadowMode;
    float InkHeight;
}

Texture2D normalTexture : register(t12);
SamplerState Sampler : register(s0);

Texture2D bloomTexture : register(t13);

struct VS_OUTPUT
{
    float4 Pos : SV_POSITION;
    float2 TexCoord : TEXCOORD0;
};

//TODO: http://www.cs.utah.edu/~reinhard/cdrom/tonemap.pdf

float4 main(VS_OUTPUT input) : SV_Target
{
    float3 color = normalTexture.Sample(Sampler, input.TexCoord).rgb;
    
    if (ScreenMode > 7) //HDR or Bloom else deferred
    {
        if (ScreenMode == 9)
        {
            color += bloomTexture.Sample(Sampler, input.TexCoord).rgb;
        }
        color = color / (color + float3(1.0f, 1.0f, 1.0f));
    }
    
    return float4(color, 1.0f);
}