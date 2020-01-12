Texture2D colorTexture : register(t26);
SamplerState Sampler : register(s0);

Texture2D depthTexture : register(t27);

Texture2D color2Texture : register(t28);
Texture2D depth2Texture : register(t29);

struct VS_OUTPUT
{
    float4 Pos : SV_POSITION;
    float2 TexCoord : TEXCOORD0;
};

struct PS_OUTPUT
{
    float4 Color : SV_Target;
    float Depth : SV_Depth;
};


PS_OUTPUT main(VS_OUTPUT input)
{
    PS_OUTPUT output = (PS_OUTPUT) 0;
    
    float4 color = colorTexture.Sample(Sampler, input.TexCoord);
    
    float depth = depthTexture.Sample(Sampler, input.TexCoord).r;
    
    if (color.a == 1.0f)
    {
        output.Color = color;
        output.Depth = depth;
        return output;
    }
    
    color = color2Texture.Sample(Sampler, input.TexCoord);
    
    depth = depth2Texture.Sample(Sampler, input.TexCoord).r;
    
    if (color.r > 0.05f)
    {
        output.Color = color;
        output.Depth = depth;
        return output;
    }
    
    output.Color = float4(0.0f, 0.0f, 0.0f, 0.0f);
    output.Depth = 1.0f;
    return output;
}
