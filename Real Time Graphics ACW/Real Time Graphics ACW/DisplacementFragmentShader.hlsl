//TODO: Note based on frank luna book

cbuffer directionalBuffer : register(b2)
{
    float4 DirectionalColor;
    float3 DirectionalDirection;
    int DirectionalUsed;
}

cbuffer pointBuffer : register(b3)
{
    float4 PointColor;
    float3 PointPosition;
    float PointAttenuationConstant;
    float PointAttenuationLinear;
    float PointAttenuationQuad;
    int PointUsed;
    float PlaneFarPlane;
}

cbuffer spotBuffer : register(b4)
{
    float4 SpotColor[4];
    float4 SpotPosition[4];
    float4 SpotDirection[4];
    float4 SpotInnerAngle;
    float4 SpotOuterAngle;
    float4 SpotAttenuationConstant;
    float4 SpotAttenuationLinear;
    float4 SpotAttenuationQuad;
    int4 SpotUsed;
}

Texture2D baseTexture : register(t6);
SamplerState Sampler : register(s0);

Texture2D specTexture : register(t7);

Texture2D normTexture : register(t8);

struct DS_OUTPUT
{
    float4 Pos : SV_POSITION;
    float3 FragmentPos : POSITION0;
    float3 ViewPosition : POSITION1;
    float2 TexCoord : TEXCOORD0;
    float3x3 TBN : POSITION2;
};

float4 main(DS_OUTPUT input) : SV_Target
{
    float3 norm = normTexture.Sample(Sampler, input.TexCoord).xyz;
    
    norm = 2.0f * norm - 1.0f;
    
    float3 normal = normalize(mul(norm, input.TBN));
    float3 viewDirection = normalize(input.ViewPosition - input.FragmentPos.xyz);
    
    float3 baseColor = specTexture.Sample(Sampler, input.TexCoord).xyz;
    float spec = specTexture.Sample(Sampler, input.TexCoord).x * 256;

    float3 color = float3(0.0f, 0.0f, 0.0f);

	//Directional Light
    if (DirectionalUsed)
    {
        float3 lightDirection = normalize(-DirectionalDirection);

        float diffuse = max(dot(normal, lightDirection), 0.0f);

        float3 reflectDirection = reflect(-lightDirection, normal);
		
        float specular = pow(max(dot(viewDirection, reflectDirection), 0.0f), spec);

        color += baseColor * DirectionalColor.xyz * float3(0.1f, 0.1f, 0.1f);
        color += baseColor * DirectionalColor.xyz * diffuse;
        color += baseColor * DirectionalColor.xyz * specular;
    }

	//Point Light
    if (PointUsed)
    {
        float3 lightDirection = normalize(PointPosition.xyz - input.FragmentPos.xyz);

        float diffuse = max(dot(normal, lightDirection), 0.0f);

        float3 reflectDirection = reflect(-lightDirection, normal);

        float specular = pow(max(dot(viewDirection, reflectDirection), 0.0f), spec);

        float distance = length(PointPosition.xyz - input.FragmentPos.xyz);
        float attenuation = 1.0f / (PointAttenuationConstant + PointAttenuationLinear * distance + PointAttenuationQuad * distance * distance);

        color += baseColor * PointColor.xyz * float3(0.1f, 0.1f, 0.1f) * attenuation;
        color += baseColor * PointColor.xyz * diffuse * attenuation;
        color += baseColor * PointColor.xyz * specular * attenuation;
    }

	//Spot Light
    for (int i = 0; i < 4; i++)
    {
        if (SpotUsed[i])
        {
            float3 lightDirection = normalize(SpotPosition[i].xyz - input.FragmentPos.xyz);
			
            float diffuse = max(dot(normal, lightDirection), 0.0f);
			
            float3 reflectDirection = reflect(-lightDirection, normal);
			
            float specular = pow(max(dot(viewDirection, reflectDirection), 0.0f), spec);
			
            float distance = length(PointPosition.xyz - input.FragmentPos.xyz);
            float attenuation = 1.0f / (SpotAttenuationConstant[i] + SpotAttenuationLinear[i] * distance + SpotAttenuationQuad[i] * distance * distance);

            float theta = dot(lightDirection, normalize(-SpotDirection[i].xyz));
            float intensity = clamp((theta - SpotOuterAngle[i]) / (SpotInnerAngle[i] - SpotOuterAngle[i]), 0.0f, 1.0f);
			
            color += baseColor * SpotColor[i].xyz * float3(0.1f, 0.1f, 0.1f) * attenuation * intensity;
            color += baseColor * SpotColor[i].xyz * diffuse * attenuation * intensity;
            color += baseColor * SpotColor[i].xyz * specular * attenuation * intensity;
        }
    }

    return float4(color, 1.0f);
}