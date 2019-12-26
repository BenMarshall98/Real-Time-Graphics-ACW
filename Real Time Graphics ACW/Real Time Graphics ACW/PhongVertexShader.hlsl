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
};

struct VS_OUTPUT
{
    float4 Pos : SV_POSITION;
    float4 FragmentPos : POSITION0;
    float4 Normal : NORMAL0;
    float3 ViewPosition : POSITION1;
    float4 LightFragmentPos : POSITION2;
};

VS_OUTPUT main(VS_INPUT input)
{
    VS_OUTPUT output = (VS_OUTPUT) 0;
    output.FragmentPos = mul(input.Pos, World);
    output.LightFragmentPos = mul(output.FragmentPos, DirectionalView);
    output.LightFragmentPos = mul(output.LightFragmentPos, DirectionalProjection);
    output.Pos = mul(output.FragmentPos, View);
    output.Pos = mul(output.Pos, Projection);
    output.Normal = mul(input.Normal, InverseWorld);
    output.ViewPosition = ViewPosition;
    return output;
}
