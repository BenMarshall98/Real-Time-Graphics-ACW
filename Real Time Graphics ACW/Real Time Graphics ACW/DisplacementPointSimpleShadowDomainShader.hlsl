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

cbuffer ShadowLightBuffer : register(b11)
{
    float3 LightPosition;
    float FarPlane;
};

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
    float4 Pos : SV_POSITION;
    float3 FragPos : POSITION0;
    float3 PosFrag : POSITION1;
    float3 LightPos : POSITION2;
};

[domain("tri")]
DS_OUTPUT main(PatchTess patch, float3 uvw : SV_DomainLocation, const OutputPatch<HS_OUTPUT, 3> tri)
{
    DS_OUTPUT output = (DS_OUTPUT) 0;
    
    float2 TexCoord = uvw[0] * tri[0].TexCoord;
    TexCoord += uvw[1] * tri[1].TexCoord;
    TexCoord += uvw[2] * tri[2].TexCoord;
    
    float height = heightTexture.SampleLevel(Sampler, TexCoord, 0).x * 0.1;
    
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
    float3 fragmentPos = mul(float4(pos, 1.0f), World).xyz;
    
    float3 lightDir = fragmentPos - LightPosition;
    
    float3 planeNormal = float3(0.0f, 1.0f, 0.0f);
    
    float d = dot(planeNormal, float3(0.0f, -4.95f, 0.0f));
    
    float t = (d - dot(planeNormal, LightPosition)) / dot(planeNormal, lightDir);
    
    output.FragPos = LightPosition + t * lightDir;
    output.PosFrag = fragmentPos;
    output.LightPos = LightPosition;
    
    output.Pos = mul(float4(output.FragPos, 1.0f), View);
    output.Pos = mul(output.Pos, Projection);
    
    return output;
}