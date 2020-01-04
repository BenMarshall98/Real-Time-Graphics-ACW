//TODO: Note based on Frank Luna Book

struct PatchTess
{
    float EdgeTess[3] : SV_TessFactor;
    float InsideTess : SV_InsideTessFactor;
};

struct VS_OUTPUT
{
    float3 Pos : POSITION0;
    float3 Normal : NORMAL0;
    float2 TexCoord : TEXCOORD0;
};

//TODO: Look at the distance from the camera

PatchTess ConstantHS(InputPatch<VS_OUTPUT, 3> inPatch, uint patchID : SV_PrimitiveID)
{
    PatchTess outPatch;
    outPatch.EdgeTess[0] = 10;
    outPatch.EdgeTess[1] = 10;
    outPatch.EdgeTess[2] = 10;
    outPatch.InsideTess = 10;
    
    return outPatch;
}

struct HS_OUTPUT
{
    float3 Pos : POSITION0;
    float3 Normal : NORMAL0;
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
    output.TexCoord = patch[i].TexCoord;
    
    return output;
}