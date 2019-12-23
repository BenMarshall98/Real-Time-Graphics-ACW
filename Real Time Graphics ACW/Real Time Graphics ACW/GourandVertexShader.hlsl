cbuffer cameraBuffer : register(b0)
{
    matrix View;
    matrix Projection;
    float3 ViewPosition;
}

cbuffer modelBuffer : register(b1)
{
    matrix World;
    matrix InverseWorld;
}

cbuffer materialBuffer : register(b2)
{
    float4 MaterialAmbient;
    float4 MaterialDiffuse;
    float4 MaterialSpecular;
    float MaterialShininess;
}

cbuffer directionalBuffer : register(b3)
{
    float4 DirectionalColor;
    float3 DirectionalDirection;
    int DirectionalUsed;
}

cbuffer pointBuffer : register(b4)
{
    float4 PointColor;
    float3 PointPosition;
    float PointAttenuationConstant;
    float PointAttenuationLinear;
    float PointAttenuationQuad;
    int PointUsed;
    float PointFarPlane;
}

cbuffer spotBuffer : register(b5)
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

struct VS_INPUT
{
    float4 Pos : POSITION;
    float4 Normal : NORMAL;
};

struct VS_OUTPUT
{
    float4 Pos : SV_POSITION;
    float4 Color : COLOR;
};

VS_OUTPUT main(VS_INPUT input)
{
    VS_OUTPUT output = (VS_OUTPUT) 0;
    output.Pos = mul(input.Pos, World);
    output.Pos = mul(output.Pos, View);
    output.Pos = mul(output.Pos, Projection);
    
    float4 fragmentPos = mul(input.Pos, World);
    
    float3 normal = normalize(mul(input.Normal, InverseWorld));
    float3 viewDirection = normalize(ViewPosition - fragmentPos.xyz);

    float3 color = float3(0.0f, 0.0f, 0.0f);

	//Directional Light
    if (DirectionalUsed)
    {
        float3 lightDirection = normalize(-DirectionalDirection);

        float diffuse = max(dot(normal, lightDirection), 0.0f);

        float3 reflectDirection = reflect(-lightDirection, normal);
		
        float specular = pow(max(dot(viewDirection, reflectDirection), 0.0f), MaterialShininess);

        color += MaterialAmbient * DirectionalColor;
        color += MaterialDiffuse * DirectionalColor * diffuse;
        color += MaterialSpecular * DirectionalColor * specular;
    }

	//Point Light
    if (PointUsed)
    {
        float3 lightDirection = normalize(PointPosition.xyz - fragmentPos.xyz);

        float diffuse = max(dot(normal, lightDirection), 0.0f);

        float3 reflectDirection = reflect(-lightDirection, normal);

        float specular = pow(max(dot(viewDirection, reflectDirection), 0.0f), MaterialShininess);

        float distance = length(PointPosition.xyz - fragmentPos.xyz);
        float attenuation = 1.0f / (PointAttenuationConstant + PointAttenuationLinear * distance + PointAttenuationQuad * distance * distance);

        color += MaterialAmbient * PointColor * attenuation;
        color += MaterialDiffuse * PointColor * diffuse * attenuation;
        color += MaterialSpecular * PointColor * specular * attenuation;
    }

	//Spot Light
    for (int i = 0; i < 4; i++)
    {
        if (SpotUsed[i])
        {
            float3 lightDirection = normalize(SpotPosition[i].xyz - fragmentPos.xyz);
			
            float diffuse = max(dot(normal, lightDirection), 0.0f);
			
            float3 reflectDirection = reflect(-lightDirection, normal);
			
            float specular = pow(max(dot(viewDirection, reflectDirection), 0.0f), MaterialShininess);
			
            float distance = length(PointPosition.xyz - fragmentPos.xyz);
            float attenuation = 1.0f / (SpotAttenuationConstant[i] + SpotAttenuationLinear[i] * distance + SpotAttenuationQuad[i] * distance * distance);

            float theta = dot(lightDirection, normalize(-SpotDirection[i].xyz));
            float intensity = clamp((theta - SpotOuterAngle[i]) / (SpotInnerAngle[i] - SpotOuterAngle[i]), 0.0f, 1.0f);
			
            color += MaterialAmbient * SpotColor[i] * attenuation * intensity;
            color += MaterialDiffuse * SpotColor[i] * diffuse * attenuation * intensity;
            color += MaterialSpecular * SpotColor[i] * specular * attenuation * intensity;
        }
    }
    
    output.Color = float4(color, 1.0f);
    
    return output;
}