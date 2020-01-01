Texture2D normalTexture : register(t6);
SamplerState normalSampler : register(s6);

Texture2D depthTexture : register(t7);
SamplerState depthSampler : register(s7);

Texture2D normalTexture2 : register(t8);
SamplerState normalSampler2 : register(s8);

Texture2D depthTexture2 : register(t9);
SamplerState depthSampler2 : register(s9);

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
    float visible = normalTexture.Sample(normalSampler, input.TexCoord).a;
    
    float visible2 = normalTexture2.Sample(normalSampler2, input.TexCoord).a;
    
    PS_OUTPUT output = (PS_OUTPUT) 0;
    
    if (visible == 0.0f && visible2 == 0.0f)
    {
        discard;
    }
    else if (visible != 0.0f && visible2 == 0.0f)
    {
        float3 color = normalTexture.Sample(normalSampler, input.TexCoord).rgb;
    
        float depth = depthTexture.Sample(depthSampler, input.TexCoord).r;
    
        output.Color = float4(color, 1.0f);
        output.Depth = depth;
    }
    else if (visible2 != 0.0f && visible == 0.0f)
    {
        float3 color = normalTexture2.Sample(normalSampler2, input.TexCoord).rgb;
    
        float depth = depthTexture2.Sample(depthSampler2, input.TexCoord).r;
    
        output.Color = float4(color, 1.0f);
        output.Depth = depth;
    }
    else
    {
        float depth = depthTexture.Sample(depthSampler, input.TexCoord).r;
        float depth2 = depthTexture2.Sample(depthSampler2, input.TexCoord).r;
        
        if (depth > depth2)
        {
            float3 color = normalTexture2.Sample(normalSampler2, input.TexCoord).rgb;
            output.Color = float4(color, 1.0f);
            output.Depth = depth2;
        }
        else
        {
            float3 color = normalTexture.Sample(normalSampler, input.TexCoord).rgb;
            output.Color = float4(color, 1.0f);
            output.Depth = depth;
        }
    }

    return output;
}