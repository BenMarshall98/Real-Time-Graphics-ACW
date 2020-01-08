//TODO: Note based on frank luna book
cbuffer cameraBuffer : register(b0)
{
    matrix View;
    matrix Projection;
    float3 ViewPosition;
}

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
    float3 Tangent : TANGENT0;
    float3 BiTangent : BITANGENT0;
};

struct DS_OUTPUT
{
    float4 Pos : SV_POSITION;
    float3 FragmentPos : POSITION0;
    float3 ViewPosition : POSITION1;
    float2 TexCoord : TEXCOORD0;
    float3x3 TBN : POSITION2;
};

[domain("tri")]
DS_OUTPUT main(PatchTess patch, float3 uvw : SV_DomainLocation, const OutputPatch<HS_OUTPUT, 3> tri)
{
    DS_OUTPUT output = (DS_OUTPUT) 0;
    
    output.TexCoord = uvw[0] * tri[0].TexCoord;
    output.TexCoord += uvw[1] * tri[1].TexCoord;
    output.TexCoord += uvw[2] * tri[2].TexCoord;
    
    float height = heightTexture.SampleLevel(Sampler, output.TexCoord, 0).x * 0.1;
    
    //Position
    float3 pos = uvw[0] * tri[0].Pos;
    pos += uvw[1] * tri[1].Pos;
    pos += uvw[2] * tri[2].Pos;
    
    //Normal
    float3 normal = uvw[0] * tri[0].Normal;
    normal += uvw[1] * tri[1].Normal;
    normal += uvw[2] * tri[2].Normal;
    
    pos += normalize(normal) * height;
    
    //FragmentPosition
    output.Pos = mul(float4(pos, 1.0f), World);
    
    output.FragmentPos = output.Pos.xyz;
    
    output.Pos = mul(output.Pos, View);
    output.Pos = mul(output.Pos, Projection);
    
    //Tangent
    float3 tangent = uvw[0] * tri[0].Tangent;
    tangent += uvw[1] * tri[1].Tangent;
    tangent += uvw[2] * tri[2].Tangent;
    
    //BiTangent
    float3 biTangent = uvw[0] * tri[0].BiTangent;
    biTangent += uvw[1] * tri[1].BiTangent;
    biTangent += uvw[2] * tri[2].BiTangent;
    
    output.TBN = float3x3(normalize(mul(World, float4(tangent, 1.0f)).xyz),
                        normalize(mul(World, float4(biTangent, 1.0f)).xyz),
                        normalize(mul(World, float4(normal, 1.0f)).xyz));
    
    //ViewPosition
    output.ViewPosition = ViewPosition;
    
    return output;
}