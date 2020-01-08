struct DS_OUTPUT
{
    float4 Pos : SV_POSITION;
    float3 FragPos : POSITION0;
};

void main(DS_OUTPUT input)
{
    if (input.FragPos.x < -5.0f || input.FragPos.x > 5.0f ||
        input.FragPos.y < -5.0f || input.FragPos.y > 5.0f)
    {
        discard;
    }
}