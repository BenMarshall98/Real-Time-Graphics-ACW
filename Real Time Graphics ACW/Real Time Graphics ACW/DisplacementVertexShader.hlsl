//TODO: Note base on Frank Luna book

cbuffer cameraBuffer : register(b0)
{
    matrix View;
    matrix Projection;
    float3 ViewPosition;
}

struct VS_OUTPUT
{
    float4 Pos : POSITION0;
    float4 FragmentPos : POSITION1;
    float4 Normal : NORMAL0;
    float3 ViewPosition : POSITION2;
    float2 TexCoord : TEXCOORD0;
};

VS_OUTPUT main(float4 Pos: POSITION, float4 Normal : NORMAL, float2 TexCoord : TEXCOORD)
{
    VS_OUTPUT output = (VS_OUTPUT) 0;
    output.Pos = Pos;
    output.Normal = Normal;
    output.FragmentPos = Pos;
    output.ViewPosition = ViewPosition;
    output.TexCoord = TexCoord;

    return output;
}