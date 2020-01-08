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
};

struct VS_OUTPUT
{
    float4 Pos : SV_Position;
    float3 FragPos : POSITION0;
};

//TODO: Source: ericson

VS_OUTPUT main(VS_INPUT input)
{
    VS_OUTPUT output = (VS_OUTPUT) 0;
    
    float3 fragmentPos = mul(input.Pos, World).xyz;
    
    float3 lightDir = DirectionalDirection;
    
    float3 planeNormal = float3(0.0f, 1.0f, 0.0f);
    
    float d = dot(planeNormal, float3(0.0f, -4.95f, 0.0f));
    
    float t = (d - dot(planeNormal, fragmentPos)) / dot(planeNormal, lightDir);
    
    output.FragPos = fragmentPos + t * lightDir;
    
    output.Pos = mul(float4(output.FragPos, 1.0f), View);
    output.Pos = mul(output.Pos, Projection);
    
    return output;
}