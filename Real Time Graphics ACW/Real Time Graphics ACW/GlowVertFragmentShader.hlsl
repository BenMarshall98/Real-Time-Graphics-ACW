//Based on https://learnopengl.com/Advanced-Lighting/Bloom

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
    float2 texOffset = float2(300.0f, 300.0f);
    
    texOffset = 1.0f / texOffset;
    
    float weight[5] =
    {
        0.227027, 0.1945946, 0.1216216, 0.054054, 0.016216
    };
    
    float4 color = colorTexture.Sample(Sampler, input.TexCoord);
    
    float depth = depthTexture.Sample(Sampler, input.TexCoord).r;
    
    PS_OUTPUT output = (PS_OUTPUT) 0;
    
    if (color.a == 1.0f)
    {
        output.Color = color;
        output.Depth = depth;
        return output;
    }
    
    color *= weight[0];
    
    float outDepth = 1.0f;
    
    for (int i = 1; i < 5; ++i)
    {
        color += colorTexture.Sample(Sampler, input.TexCoord + float2(0.0, texOffset.y * i)) * weight[i];
        color += colorTexture.Sample(Sampler, input.TexCoord - float2(0.0, texOffset.y * i)) * weight[i];
        
        if (outDepth == 1.0f)
        {
            outDepth = depthTexture.Sample(Sampler, input.TexCoord + float2(0.0, texOffset.y * i)).r;
        }
        else
        {
            depth = depthTexture.Sample(Sampler, input.TexCoord + float2(0.0, texOffset.y * i)).r;
            
            if (depth < 1.0f)
            {
                if (depth > outDepth)
                {
                    outDepth = depth;
                }
            }
        }
        
        if (outDepth == 1.0f)
        {
            outDepth = depthTexture.Sample(Sampler, input.TexCoord - float2(0.0, texOffset.y * i)).r;
        }
        else
        {
            depth = depthTexture.Sample(Sampler, input.TexCoord - float2(0.0, texOffset.y * i)).r;
            
            if (depth < 1.0f)
            {
                if (depth > outDepth)
                {
                    outDepth = depth;
                }
            }
        }
    }
    
    output.Color = float4(color.xyz, 1.0f);
    output.Depth = outDepth;

    return output;
}