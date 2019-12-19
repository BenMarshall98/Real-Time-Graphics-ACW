struct VS_OUTPUT
{
    float4 pos : SV_Position;
    float2 texCoord : TEXCOORD0;
    float particleTime : BLENDWEIGHT0;
};

float4 main(VS_OUTPUT input) : SV_Target
{
    return float4(input.particleTime, input.particleTime, input.particleTime, 1.0);
}