Texture2D normalTexture : register(t12);
SamplerState Sampler : register(s0);

Texture2D depthTexture : register(t13);

Texture2D normalTexture2 : register(t14);

Texture2D depthTexture2 : register(t15);

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
    float visible = normalTexture.Sample(Sampler, input.TexCoord).a;
    
    float visible2 = normalTexture2.Sample(Sampler, input.TexCoord).a;
    
    PS_OUTPUT output = (PS_OUTPUT) 0;
    
    if (visible == 0.0f && visible2 == 0.0f)
    {
        discard;
    }
    else if (visible != 0.0f && visible2 == 0.0f)
    {
        float4 color = normalTexture.Sample(Sampler, input.TexCoord).rgba;
    
        float depth = depthTexture.Sample(Sampler, input.TexCoord).r;
    
        output.Color = float4(color);
        output.Depth = depth;
    }
    else if (visible2 != 0.0f && visible == 0.0f)
    {
        float4 color = normalTexture2.Sample(Sampler, input.TexCoord).rgba;
    
        float depth = depthTexture2.Sample(Sampler, input.TexCoord).r;
    
        output.Color = float4(color);
        output.Depth = depth;
    }
    else
    {
        float depth = depthTexture.Sample(Sampler, input.TexCoord).r;
        float depth2 = depthTexture2.Sample(Sampler, input.TexCoord).r;
        
        if (depth > depth2)
        {
            float4 color = normalTexture2.Sample(Sampler, input.TexCoord).rgba;
            output.Color = float4(color);
            output.Depth = depth2;
        }
        else
        {
            float4 color = normalTexture.Sample(Sampler, input.TexCoord).rgba;
            output.Color = float4(color);
            output.Depth = depth;
        }
    }

    return output;
}