//TODO: Note based on frank luna book
//TODO: Sort out normal in this shader
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

Texture2D baseTexture : register(t0);
SamplerState baseSampler : register(s0);

struct PatchTess
{
    float EdgeTess[3] : SV_TessFactor;
    float InsideTess : SV_InsideTessFactor;
};

struct HS_OUTPUT
{
    float4 Pos : POSITION0;
    float4 FragmentPos : POSITION1;
    float4 Normal : NORMAL0;
    float3 ViewPosition : POSITION2;
    float2 TexCoord : TEXCOORD0;
};

struct DS_OUTPUT
{
    float4 Pos : SV_POSITION;
    float4 FragmentPos : POSITION0;
    float4 Normal : NORMAL0;
    float3 ViewPosition : POSITION1;
};

[domain("tri")]
DS_OUTPUT main(PatchTess patch, float3 uvw : SV_DomainLocation, const OutputPatch<HS_OUTPUT, 3> tri)
{
    DS_OUTPUT output = (DS_OUTPUT) 0;
    
    //Position
    output.Pos = uvw[0] * tri[0].Pos;
    output.Pos += uvw[1] * tri[1].Pos;
    output.Pos += uvw[2] * tri[2].Pos;
    
    output.Pos = mul(output.Pos, World);
    output.Pos = mul(output.Pos, View);
    output.Pos = mul(output.Pos, Projection);
    
    //FragmentPosition
    output.FragmentPos = uvw[0] * tri[0].FragmentPos;
    output.FragmentPos += uvw[1] * tri[1].FragmentPos;
    output.FragmentPos += uvw[2] * tri[2].FragmentPos;
    
    output.FragmentPos = mul(output.FragmentPos, World);
    
    //Normal
    output.Normal = uvw[0] * tri[0].Normal;
    output.Normal += uvw[1] * tri[1].Normal;
    output.Normal += uvw[2] * tri[2].Normal;
    
    output.Normal = mul(output.Normal, InverseWorld);
    
    //ViewPosition
    output.ViewPosition = tri[0].ViewPosition;
    
    return output;
}