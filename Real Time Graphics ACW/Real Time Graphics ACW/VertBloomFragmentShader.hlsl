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
    float2 texOffset;
    
    colorTexture.GetDimensions(texOffset.x, texOffset.y);
    
    texOffset = 1.0f / texOffset;
    
    float weight[5] =
    {
        0.227027, 0.1945946, 0.1216216, 0.054054, 0.016216
    };
    
    float3 color = colorTexture.Sample(Sampler, input.TexCoord).rgb * weight[0];
    
    
    for (int i = 1; i < 5; ++i)
    {
        color += colorTexture.Sample(Sampler, input.TexCoord + float2(0.0, texOffset.y * i)).rgb * weight[i];
        color += colorTexture.Sample(Sampler, input.TexCoord - float2(0.0, texOffset.y * i)).rgb * weight[i];
    }

    return float4(color, 1.0f);
}