cbuffer inkBuffer : register(b0) //TODO: register
{
    matrix InkView;
    matrix InkProjection;
}

cbuffer modelBuffer : register(b1)
{
    matrix World;
    matrix InverseWorld;
}

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
    float3 FragmentPos : POSITION0;
    float3 HeightPos : POSITION1;
};

[domain("tri")]
DS_OUTPUT main(PatchTess patch, float3 uvw : SV_DomainLocation, const OutputPatch<HS_OUTPUT, 3> tri)
{
    DS_OUTPUT output = (DS_OUTPUT) 0;
    
    float2 texCoord = uvw[0] * tri[0].TexCoord;
    texCoord += uvw[1] * tri[1].TexCoord;
    texCoord += uvw[2] * tri[2].TexCoord;
    
    //Position
    float3 pos = uvw[0] * tri[0].Pos;
    pos += uvw[1] * tri[1].Pos;
    pos += uvw[2] * tri[2].Pos;
    
    //Normal
    float3 normal = uvw[0] * tri[0].Normal;
    normal += uvw[1] * tri[1].Normal;
    normal += uvw[2] * tri[2].Normal;
    
    float3 heightPos = pos + normal * texCoord.x * texCoord.y;
    
    output.Pos = mul(float4(pos, 1.0f), World);
    output.FragmentPos = output.Pos.xyz;
    output.Pos = mul(output.Pos, InkView);
    output.Pos = mul(output.Pos, InkProjection);
    
    output.HeightPos = mul(float4(heightPos, 1.0f), World).xyz;
    
    return output;
}