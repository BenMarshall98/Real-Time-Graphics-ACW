cbuffer cameraBuffer : register(b0)
{
    matrix View;
    matrix Projection;
    float3 ViewPosition;
}

struct VS_INPUT
{
    float2 position : POSITION;
    float2 texCoord : TEXCOORD;
    float3 particlePosition : PARTICLEPOSITION;
    float particleTime : PARTICLETIME;
};
    
struct VS_OUTPUT
{
    float4 pos : SV_Position;
    float2 texCoord : TEXCOORD0;
    float particleTime : BLENDWEIGHT0;
};

VS_OUTPUT main(VS_INPUT input)
{
    VS_OUTPUT output = (VS_OUTPUT) 0;
    
    float3 cameraLeft = float3(View[0][0], View[1][0], View[2][0]);
    float3 cameraUp = float3(View[0][1], View[1][1], View[2][1]);
    
    float3 pos = input.position.x * 0.1f * cameraLeft + input.position.y * 0.1f * cameraUp;
    pos += input.particlePosition;
    
    output.pos = mul(float4(pos, 1.0f), View);
    output.pos = mul(output.pos, Projection);
    output.texCoord = input.texCoord;
    output.particleTime = input.particleTime;
    
    return output;
}