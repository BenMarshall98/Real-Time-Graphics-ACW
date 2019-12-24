struct DS_OUTPUT
{
    float4 Pos : SV_POSITION;
    float3 FragmentPos : POSITION0;
    float3 HeightPos : POSITION1;
};

float main(DS_OUTPUT input) : SV_Depth
{
    return input.HeightPos.y;
}