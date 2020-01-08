struct VS_OUTPUT
{
    float4 Pos : SV_Position;
    float3 FragPos : POSITION0;
};

void main(VS_OUTPUT input)
{
    if (input.FragPos.x < -5.0f || input.FragPos.x > 5.0f ||
        input.FragPos.y < -5.0f || input.FragPos.y > 5.0f)
    {
        discard;
    }
}