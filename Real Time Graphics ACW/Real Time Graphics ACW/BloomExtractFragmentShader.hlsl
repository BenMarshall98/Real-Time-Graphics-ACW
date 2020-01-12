//Based on https://learnopengl.com/Advanced-Lighting/Bloom

Texture2D colorTexture : register(t0);
SamplerState Sampler : register(s0);

struct VS_OUTPUT
{
    float4 Pos : SV_POSITION;
    float2 TexCoord : TEXCOORD0;
};

float4 main(VS_OUTPUT input) : SV_Target
{
    float3 color = colorTexture.Sample(Sampler, input.TexCoord).rgb;
    
    float brightness = dot(color, float3(0.2126f, 0.7152f, 0.0722f));
    
    if (brightness > 1.0)
    {
        return float4(color, 1.0);
    }
    else
    {
        return float4(0.0, 0.0, 0.0, 1.0);
    }
}