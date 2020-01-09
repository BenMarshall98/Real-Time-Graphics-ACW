//TODO: Source: LearnOpenGL

Texture2D colorTexture : register(t26);
SamplerState Sampler : register(s0);

Texture2D depthTexture : register(t27);

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
    float alpha = colorTexture.Sample(Sampler, input.TexCoord).a;
    
    float depth = depthTexture.Sample(Sampler, input.TexCoord).r;
    
    PS_OUTPUT output = (PS_OUTPUT) 0;
    
    if (alpha == 1.0f)
    {
        output.Color = float4(1.0f, 1.0f, 0.0f, 1.0f);
    }
    else
    {
        output.Color = float4(0.0f, 0.0f, 0.0f, 0.0f);
    }
       
    output.Depth = depth;
    
    return output;
}