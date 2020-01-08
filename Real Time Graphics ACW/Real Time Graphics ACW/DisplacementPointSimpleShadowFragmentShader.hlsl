struct DS_OUTPUT
{
    float4 Pos : SV_POSITION;
    float3 FragPos : POSITION0;
    float3 PosFrag : POSITION1;
    float3 LightPos : POSITION2;
};

void main(DS_OUTPUT input)
{
    if (input.PosFrag.y >= input.LightPos.y - 0.5f)
    {
        discard;
    }
    
    if (input.FragPos.x < -5.0f || input.FragPos.x > 5.0f ||
        input.FragPos.y < -5.0f || input.FragPos.y > 5.0f)
    {
        discard;
    }
}