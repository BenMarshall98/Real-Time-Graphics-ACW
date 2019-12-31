Texture2D baseTexture : register(t6);
SamplerState baseSampler : register(s6);

Texture2D specTexture : register(t7);
SamplerState specSampler : register(s7);

struct VS_OUTPUT
{
    float4 Pos : SV_POSITION;
    float4 FragmentPos : POSITION0;
    float4 Normal : NORMAL0;
    float4 LightFragmentPos : POSITION1;
    float2 TexCoord : TEXCOORD0;
};

struct PS_OUTPUT
{
    float4 Pos : SV_Target0;
    float4 LightPos : SV_Target1;
    float4 LightNorm : SV_Target2;
    float4 Normal : SV_Target3;
    float4 Ambient : SV_Target4;
    float4 Diffuse : SV_Target5;
    float4 Specular : SV_Target6;
};

PS_OUTPUT main(VS_OUTPUT input)
{
    PS_OUTPUT output = (PS_OUTPUT) 0;

    output.Pos = input.FragmentPos;
    output.LightPos = input.LightFragmentPos;
    output.LightNorm = input.Normal;
    output.Normal = input.Normal;
    
    float3 color = baseTexture.Sample(baseSampler, input.TexCoord).rgb;
    float spec = specTexture.Sample(specSampler, input.TexCoord).r * 256;
    
    output.Ambient = float4(color * 0.1f, 1.0f);
    output.Diffuse = float4(color, 1.0f);
    output.Specular = float4(color, spec);
    
    return output;
}