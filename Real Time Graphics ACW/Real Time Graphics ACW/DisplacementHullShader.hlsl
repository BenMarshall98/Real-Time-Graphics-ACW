//TODO: Note based on frank luna book

struct PatchTess
{
    float EdgeTess[3] : SV_TessFactor;
    float InsideTess : SV_InsideTessFactor;
};

struct VS_OUTPUT
{
    float4 Pos : POSITION0;
    float4 FragmentPos : POSITION1;
    float4 Normal : NORMAL0;
    float3 ViewPosition : POSITION2;
    float2 TexCoord : TEXCOORD0;
};

PatchTess ConstantHS(InputPatch<VS_OUTPUT, 3> inPatch, uint patchID: SV_PrimitiveID)
{
    PatchTess outPatch;
    outPatch.EdgeTess[0] = 3;
    outPatch.EdgeTess[1] = 3;
    outPatch.EdgeTess[2] = 3;
    outPatch.InsideTess = 3;
    
    return outPatch;
}

struct HS_OUTPUT
{
    float4 Pos : POSITION0;
    float4 FragmentPos : POSITION1;
    float4 Normal : NORMAL0;
    float3 ViewPosition : POSITION2;
    float2 TexCoord : TEXCOORD0;
};

[domain("tri")]
[partitioning("integer")]
[outputtopology("triangle_cw")]
[outputcontrolpoints(3)]
[patchconstantfunc("ConstantHS")]
[maxtessfactor(64.0f)]

HS_OUTPUT main(InputPatch<VS_OUTPUT, 3> patch, uint i : SV_OutputControlPointID, uint patchID : SV_PrimitiveID)
{
    HS_OUTPUT output = (HS_OUTPUT) 0;
    
    output.Pos = patch[i].Pos;
    output.Normal = patch[i].Normal;
    output.FragmentPos = patch[i].FragmentPos;
    output.ViewPosition = patch[i].ViewPosition;
    output.TexCoord = patch[i].TexCoord;
    
    return output;
}