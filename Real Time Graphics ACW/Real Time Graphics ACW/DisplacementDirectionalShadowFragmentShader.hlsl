struct DS_OUTPUT
{
    float4 Pos : SV_POSITION;
};

float4 main(DS_OUTPUT input) : SV_Target
{
    float depth = input.Pos.z / input.Pos.w;
    
    //TODO: Source: https://fabiensanglard.net/shadowmappingVSM/
    float moment1 = depth;
    float moment2 = depth * depth;
    
    float dx = ddx(depth);
    float dy = ddy(depth);
    
    moment2 += 0.25f * (dx * dx + dy * dy);
    
    return float4(moment1, moment2, 0.0f, 1.0f);
}