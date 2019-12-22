//TODO: Source

cbuffer pointLightMatrixBuffer : register(b0)
{
    matrix PointLightPerspective;
    matrix PointLightView[6];
}

struct DS_OUTPUT
{
    float4 Pos : POSITION0;
};

struct GS_OUTPUT
{
    float4 Pos : SV_Position;
    float3 FragPos : POSITION0;
    uint Layer : SV_RenderTargetArrayIndex;
};

[maxvertexcount(18)]
void main(triangle DS_OUTPUT input[3], inout TriangleStream<GS_OUTPUT> output)
{
    for (uint i = 0; i < 6; ++i)
    {
        for (uint j = 0; j < 3; ++j)
        {
            GS_OUTPUT temp;
            temp.Layer = i;
            temp.FragPos = input[j].Pos.xyz;
            temp.Pos = mul(input[j].Pos, PointLightView[i]);
            temp.Pos = mul(temp.Pos, PointLightPerspective);
            
            output.Append(temp);
        }
        output.RestartStrip();
    }
}