Texture2D baseTexture : register(t6);
SamplerState baseSampler : register(s6);

Texture2D specTexture : register(t7);
SamplerState specSampler : register(s7);

Texture2D normTexture : register(t8);
SamplerState normSampler : register(s8);

struct DS_OUTPUT
{
    float4 Pos : SV_POSITION;
    float3 FragmentPos : POSITION0;
    float4 LightFragmentPos : POSITION1;
    float2 TexCoord : TEXCOORD0;
    float3x3 TBN : POSITION2;
};

struct PS_OUTPUT
{
    float4 Pos : SV_Target0;
    float4 LightPos : SV_Target1;
    float4 Normal : SV_Target2;
    float4 Ambient : SV_Target3;
    float4 Diffuse : SV_Target4;
    float4 Specular : SV_Target5;
};

PS_OUTPUT main(DS_OUTPUT input)
{
    float3 norm = normTexture.Sample(normSampler, input.TexCoord).xyz;
    
    norm = 2.0f * norm - 1.0f;
    
    float3 normal = normalize(mul(norm, input.TBN));
    
    float3 baseColor = specTexture.Sample(specSampler, input.TexCoord).xyz;
    float spec = specTexture.Sample(specSampler, input.TexCoord).x * 256;
    
    PS_OUTPUT output = (PS_OUTPUT) 0;
    
    output.Pos = float4(input.FragmentPos, 1.0f);
    output.LightPos = input.LightFragmentPos;
    output.Normal = float4(normal, 1.0f);
    output.Ambient = float4(baseColor * 0.1f, 1.0f);
    output.Diffuse = float4(baseColor, 1.0f);
    output.Specular = float4(baseColor, spec);
    
    return output;
}