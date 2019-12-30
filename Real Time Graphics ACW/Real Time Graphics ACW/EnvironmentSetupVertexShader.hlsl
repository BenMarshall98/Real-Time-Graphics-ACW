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
    float4 Pos : POSITION0;
    float4 Normal : NORMAL0;
    float4 LightFragmentPos : POSITION1;
    float3 ViewPosition : POSITON2;
};

VS_OUTPUT main(VS_INPUT input)
{
    VS_OUTPUT output = (VS_OUTPUT) 0;
    output.Pos = mul(input.Pos, World);
    output.Normal = mul(input.Normal, InverseWorld);
    output.LightFragmentPos = mul(output.Pos, DirectionalView);
    output.LightFragmentPos = mul(output.LightFragmentPos, DirectionalProjection);
    output.ViewPosition = ViewPosition;

    return output;
}