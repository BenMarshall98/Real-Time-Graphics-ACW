cbuffer modelBuffer : register(b1)
{
    matrix World;
    matrix InverseWorld;
}

struct VS_INPUT
{
    float3 Pos : POSITION;
};

struct VS_OUTPUT
{
    float4 Pos : POSITION0;
};

VS_OUTPUT main(VS_INPUT input)
{
    VS_OUTPUT output = (VS_OUTPUT) 0;
    
    output.Pos = mul(float4(input.Pos, 1.0f), World);
    
    return output;
}