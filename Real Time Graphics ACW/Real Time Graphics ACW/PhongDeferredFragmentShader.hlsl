cbuffer materialBuffer : register(b1)
{
    float4 MaterialAmbient;
    float4 MaterialDiffuse;
    float4 MaterialSpecular;
    float MaterialShininess;
}

struct VS_OUTPUT
{
    float4 Pos : SV_POSITION;
    float4 FragmentPos : POSITION0;
    float4 Normal : NORMAL0;
    float4 LightFragmentPos : POSITION1;
};

struct PS_OUTPUT
{
    float4 Pos : SV_Target0;
    float4 LightPos : SV_Target1;
    float4 Normal : SV_Target2;
    float4 Ambient : SV_Target3;
    float4 Diffuse : SV_Target4;
    float4 Specular : SV_Target5;
};

PS_OUTPUT main(VS_OUTPUT input)
{
    PS_OUTPUT output = (PS_OUTPUT) 0;
    
    output.Pos = input.FragmentPos;
    output.LightPos = input.LightFragmentPos;
    output.Normal = input.Normal;
    output.Ambient = MaterialAmbient;
    output.Diffuse = MaterialDiffuse;
    output.Specular = float4(MaterialSpecular.xyz, MaterialShininess);
    
    return output;
}