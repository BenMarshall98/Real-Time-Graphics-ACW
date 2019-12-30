Texture2D baseTexture : register(t6);
SamplerState baseSampler : register(s6);

Texture2D specTexture : register(t7);
SamplerState specSampler : register(s7);

Texture2D normTexture : register(t8);
SamplerState normSampler : register(s8);

struct VS_OUTPUT
{
    float4 Pos : SV_POSITION;
    float3 FragmentPos : POSITION0;
    float2 TexCoord : TEXCOORD0;
    float4 LightFragmentPos : POSITION1;
    float3x3 TBN : POSITION2;
};


struct PS_OUTPUT
{
    float4 Pos : SV_Target0;
    float4 Normal : SV_Target1;
    float4 Ambient : SV_Target2;
    float4 Diffuse : SV_Target3;
    float4 Specular : SV_Target4;
};

PS_OUTPUT main(VS_OUTPUT input)
{
    //TODO: Parallax
    
    float3 norm = normTexture.Sample(normSampler, input.TexCoord).xyz;
    
    norm = 2.0f * norm - 1.0f;
    
    float3 normal = normalize(mul(norm, input.TBN));
    
    float3 baseColor = specTexture.Sample(specSampler, input.TexCoord).xyz;
    float spec = specTexture.Sample(specSampler, input.TexCoord).x * 256;
    
    PS_OUTPUT output = (PS_OUTPUT) 0;
    
    output.Pos = input.FragmentPos;
    output.Normal = normal;
    output.Ambient = float4(baseColor * 0.1f, 1.0f);
    output.Diffuse = float4(baseColor, 1.0f);
    output.Specular = float4(baseColor, spec);
    
    return output;
}