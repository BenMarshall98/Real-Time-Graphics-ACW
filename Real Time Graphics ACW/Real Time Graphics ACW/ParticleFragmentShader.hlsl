Texture2D baseTexture : register(t25);
SamplerState Sampler : register(s0);

struct VS_OUTPUT
{
    float4 pos : SV_Position;
    float2 texCoord : TEXCOORD0;
    float particleTime : BLENDWEIGHT0;
};

float4 main(VS_OUTPUT input) : SV_Target
{
    float col = baseTexture.Sample(Sampler, input.texCoord).r;
    
    float3 color = float3(0.0f, 0.0f, 0.0f);
    
    if (input.particleTime > 0.5)
    {
        color = lerp(float3(1.0f, 1.0f, 1.0f), float3(1.0f, 1.0f, 0.0f), (input.particleTime - 0.5) * 2);
    }
    else if (input.particleTime > 0.25)
    {
        color = lerp(float3(1.0f, 1.0f, 0.0f), float3(1.0f, 0.0f, 0.0f), (input.particleTime - 0.25) * 4);
    }
    else if (input.particleTime > 0.0f)
    {
        color = lerp(float3(1.0f, 0.0f, 0.0f), float3(0.0f, 0.0f, 0.0f), input.particleTime * 4);
    }
    
    float distance = length(input.texCoord.xy * 2.0f - 1.0f);
    
    distance = saturate(distance);
    
    distance = 1.0f - distance;
    
    return float4(color * col, distance);
}