cbuffer modelBuffer : register(b1)
{
    matrix World;
    matrix inverseWorld;
}

cbuffer inkPlaneBuffer : register(b6)
{
    matrix inkPlaneView;
    matrix inkPlaneProjection;
}

cbuffer inkBuffer : register(b7)
{
    matrix inkWorld;
    matrix inkView;
    matrix inkProjection;
    float inkHeight;
}

struct VS_INPUT
{
    float4 Pos : POSITION;
};

struct VS_OUTPUT
{
    float4 Pos : SV_POSITION;
    float3 FragPos : POSITION0;
    float3 InkCenter : POSITION1;
    float3 InkXDir : POSITION2;
    float3 InkZDir : POSITION3;
};

VS_OUTPUT main(VS_INPUT input)
{
    VS_OUTPUT output = (VS_OUTPUT) 0;
    output.Pos = mul(input.Pos, World);
    output.FragPos = output.Pos.xyz;
    output.Pos = mul(output.Pos, inkPlaneView);
    output.Pos = mul(output.Pos, inkPlaneProjection);

    output.InkCenter = mul(float4(0.0f, 0.0f, 0.0f, 1.0f), inkWorld);
    output.InkXDir = mul(float4(1.0f, 0.0f, 0.0f, 1.0f), inkWorld);
    output.InkZDir = mul(float4(0.0f, 0.0f, 1.0f, 1.0f), inkWorld);

    return output;
}