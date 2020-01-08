//TODO: Note based on frank luna book

cbuffer modelBuffer : register(b1)
{
    matrix World;
    matrix InverseWorld;
}

Texture2D heightTexture : register(t0);
SamplerState Sampler : register(s0);

struct PatchTess
{
    float EdgeTess[3] : SV_TessFactor;
    float InsideTess : SV_InsideTessFactor;
};

struct HS_OUTPUT
{
    float3 Pos : POSITION0;
    float3 Normal : NORMAL0;
    float2 TexCoord : TEXCOORD0;
};

struct DS_OUTPUT
{
    float4 Pos : POSITION0;
};

[domain("tri")]
DS_OUTPUT main(PatchTess patch, float3 uvw : SV_DomainLocation, const OutputPatch<HS_OUTPUT, 3> tri)
{
    DS_OUTPUT output = (DS_OUTPUT) 0;
    
    float2 texCoord = uvw[0] * tri[0].TexCoord;
    texCoord += uvw[1] * tri[1].TexCoord;
    texCoord += uvw[2] * tri[2].TexCoord;
    
    float height = heightTexture.SampleLevel(Sampler, texCoord, 0).x * 0.1;
    
    float3 pos = uvw[0] * tri[0].Pos;
    pos += uvw[1] * tri[1].Pos;
    pos += uvw[2] * tri[2].Pos;
    
    float3 normal = uvw[0] * tri[0].Normal;
    normal += uvw[1] * tri[1].Normal;
    normal += uvw[2] * tri[2].Normal;
    
    pos += normalize(normal) * height;

    output.Pos = mul(float4(pos, 1.0f), World);
    
    return output;
}