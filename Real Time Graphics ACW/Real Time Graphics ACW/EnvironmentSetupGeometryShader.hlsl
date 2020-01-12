//Based on https://learnopengl.com/Advanced-Lighting/Shadows/Point-Shadows

cbuffer environmentMatrixBuffer : register(b4)
{
    matrix EnvironmentView[6];
    matrix EnvironmentPerspective;
}

struct VS_OUTPUT
{
    float4 Pos : POSITION0;
    float4 Normal : NORMAL0;
    float4 LightFragmentPos : POSITION1;
    float3 ViewPosition : POSITION2;
    float4 Position : POSITION3;
};

struct GS_OUTPUT
{
    float4 Pos : SV_Position;
    float3 FragPos : POSITION0;
    float4 Normal : NORMAL0;
    float4 LightFragmentPos : POSITION1;
    float3 ViewPosition : POSITION2;
    float4 Position : POSITION3;
    uint Layer : SV_RenderTargetArrayIndex;
};

[maxvertexcount(18)]
void main(triangle VS_OUTPUT input[3], inout TriangleStream<GS_OUTPUT> output)
{
    for (uint i = 0; i < 6; ++i)
    {
        for (uint j = 0; j < 3; ++j)
        {
            GS_OUTPUT temp = (GS_OUTPUT) 0;
            
            temp.Layer = i;
            temp.FragPos = input[j].Pos.xyz;
            temp.Pos = mul(input[j].Pos, EnvironmentView[i]);
            temp.Pos = mul(temp.Pos, EnvironmentPerspective);
            temp.Position = input[j].Position;
            temp.Normal = input[j].Normal;
            temp.ViewPosition = input[j].ViewPosition;
            output.Append(temp);
        }
        output.RestartStrip();
    }
}