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
Texture2D inkTexture : register(t0); //TODO: register
SamplerState inkSampler : register(s0);

struct PatchTess
{
    float EdgeTess[3] : SV_TEssFactor;
    float InsideTess : SV_InsideTessFactor;
};

struct HS_OUTPUT
{
    float3 Pos : POSITION0;
    float3 Normal : NORMA0;
    float2 TexCoord : TEXCOORD0;
};

struct DS_OUTPUT
{
    float4 Pos : SV_POSITION;
};

[domain("tri")]
DS_OUTPUT main(PatchTess patch, float3 uvw : SV_DomainLocation, const OutputPatch<HS_OUTPUT, 3> tri)
{
    DS_OUTPUT output = (DS_OUTPUT) 0;
    
    float2 texCoord = uvw[0] * tri[0].TexCoord;
    texCoord += uvw[1] * tri[1].TexCoord;
    texCoord += uvw[2] * tri[2].TexCoord;
    
    float height = inkTexture.SampleLevel(inkSampler, texCoord, 0).x;
    
    //Position
    float3 pos = uvw[0] * tri[0].Pos;
    pos += uvw[1] * tri[1].Pos;
    pos += uvw[2] * tri[2].Pos;
    
    //Normal
    float3 normal = uvw[0] * tri[0].Normal;
    normal += uvw[1] * tri[1].Normal;
    normal += uvw[2] * tri[2].Normal;
    
    float3 heightPos = pos + normal * height;
    
    output.Pos = mul(float4(heightPos, 1.0f), World);
    output.Pos = mul(output.Pos, View);
    output.Pos = mul(output.Pos, Projection);
    
    return output;
}