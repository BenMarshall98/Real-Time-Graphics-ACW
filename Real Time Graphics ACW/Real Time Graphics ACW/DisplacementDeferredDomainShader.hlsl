//Based on example in Frank Luna's Introduction to 3D Game Programming with DirectX11

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

cbuffer directionalBuffer : register(b3)
{
    float4 DirectionalColor;
    float3 DirectionalDirection;
    int DirectionalUsed;
    matrix DirectionalView;
    matrix DirectionalProjection;
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
    float4 LightFragmentPos : POSITION1;
    float3 Normal : NORMAL0;
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
    output.Normal = uvw[0] * tri[0].Normal;
    output.Normal += uvw[1] * tri[1].Normal;
    output.Normal += uvw[2] * tri[2].Normal;
    
    pos += normalize(output.Normal) * height;
    
    //FragmentPosition
    output.Pos = mul(float4(pos, 1.0f), World);
    
    output.FragmentPos = output.Pos.xyz;
    
    output.LightFragmentPos = mul(output.Pos, DirectionalView);
    output.LightFragmentPos = mul(output.LightFragmentPos, DirectionalProjection);
    
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
    
    output.TBN = float3x3(normalize(mul(float4(normalize(tangent), 1.0f), InverseWorld).xyz),
                        normalize(mul(float4(normalize(biTangent), 1.0f), InverseWorld).xyz),
                        normalize(mul(float4(normalize(output.Normal), 1.0f), InverseWorld).xyz));
    
    return output;
}