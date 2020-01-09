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

struct VS_INPUT
{
    float4 Pos : POSITION;
    float4 Normal : NORMAL;
    float2 TexCoord : TEXCOORD;
};

struct VS_OUTPUT
{
    float4 Pos : SV_POSITION;
    float4 FragmentPos : POSITION0;
    float4 Normal : NORMAL0;
    float3 ViewPosition : POSITION1;
    float2 TexCoord : TEXCOORD0;
    float4 LightFragmentPos : POSITION2;
    float4 Position : POSITION3;
    float3 PosDir : POSITION4;
};

VS_OUTPUT main(VS_INPUT input)
{
    VS_OUTPUT output = (VS_OUTPUT) 0;
    output.Pos = mul(input.Pos, World);
    output.Pos = mul(output.Pos, View);
    output.Pos = mul(output.Pos, Projection);
    output.FragmentPos = mul(input.Pos, World);
    output.LightFragmentPos = mul(output.FragmentPos, DirectionalView);
    output.LightFragmentPos = mul(output.LightFragmentPos, DirectionalProjection);
    output.Normal = mul(input.Normal, InverseWorld);
    output.ViewPosition = ViewPosition;
    output.TexCoord = input.TexCoord;
    output.Position = output.Pos;
    output.PosDir = output.FragmentPos.xyz - mul(float4(0.0f, 0.0f, 0.0f, 1.0f), World).xyz;
    return output;
}
