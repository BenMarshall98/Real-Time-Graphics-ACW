cbuffer materialBuffer : register(b1)
{
    float4 MaterialAmbient;
    float4 MaterialDiffuse;
    float4 MaterialSpecular;
    float MaterialShininess;
}

cbuffer directionalBuffer : register(b2)
{
    float4 DirectionalColor;
    float3 DirectionalDirection;
    int DirectionalUsed;
}

cbuffer pointBuffer : register(b3)
{
    float4 PointColor;
    float4 PointPosition;
    float PointAttenuationConstant;
    float PointAttenuationLinear;
    float PointAttenuationQuad;
    int PointUsed;
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

struct VS_OUTPUT
{
    float4 Pos : SV_POSITION;
    float4 FragmentPos : POSITION0;
    float4 Normal : NORMAL0;
    float3 ViewPosition : POSITION1;
};

float4 main(VS_OUTPUT input) : SV_Target
{
    float3 normal = normalize(input.Normal.xyz);
    float3 viewDirection = normalize(input.ViewPosition - input.FragmentPos.xyz);

    float3 color = float3(0.0f, 0.0f, 0.0f);

	//Directional Light
    if (DirectionalUsed)
    {
        float3 lightDirection = normalize(-DirectionalDirection);

        float diffuse = max(dot(normal, lightDirection), 0.0f);

        float3 reflectDirection = reflect(-lightDirection, normal);
		
        float specular = pow(max(dot(viewDirection, reflectDirection), 0.0f), MaterialShininess);
        
        if (diffuse > 0.9f)
        {
            diffuse = 0.9f;
        }
        else if(diffuse > 0.5f)
        {
            diffuse = 0.5f;
        }
        else if(diffuse > 0.25f)
        {
            diffuse = 0.25f;
        }
        else
        {
            diffuse = 0.0f;
        }
        
        if (specular > 0.9f)
        {
            specular = 0.9f;
        }
        else if (specular > 0.5f)
        {
            specular = 0.5f;
        }
        else if (specular > 0.25f)
        {
            specular = 0.25f;
        }
        else
        {
            specular = 0.0f;
        }

        color += MaterialAmbient * DirectionalColor;
        color += MaterialDiffuse * DirectionalColor * diffuse;
        color += MaterialSpecular * DirectionalColor * specular;
    }

	//Point Light
    if (PointUsed)
    {
        float3 lightDirection = normalize(PointPosition.xyz - input.FragmentPos.xyz);

        float diffuse = max(dot(normal, lightDirection), 0.0f);

        float3 reflectDirection = reflect(-lightDirection, normal);

        float specular = pow(max(dot(viewDirection, reflectDirection), 0.0f), MaterialShininess);

        float distance = length(PointPosition.xyz - input.FragmentPos.xyz);
        float attenuation = 1.0f / (PointAttenuationConstant + PointAttenuationLinear * distance + PointAttenuationQuad * distance * distance);
        
        if (diffuse > 0.9f)
        {
            diffuse = 0.9f;
        }
        else if (diffuse > 0.5f)
        {
            diffuse = 0.5f;
        }
        else if (diffuse > 0.25f)
        {
            diffuse = 0.25f;
        }
        else
        {
            diffuse = 0.0f;
        }
        
        if (specular > 0.9f)
        {
            specular = 0.9f;
        }
        else if (specular > 0.5f)
        {
            specular = 0.5f;
        }
        else if (specular > 0.25f)
        {
            specular = 0.25f;
        }
        else
        {
            specular = 0.0f;
        }

        color += MaterialAmbient * PointColor * attenuation;
        color += MaterialDiffuse * PointColor * diffuse * attenuation;
        color += MaterialSpecular * PointColor * specular * attenuation;
    }

	//Spot Light
    for (int i = 0; i < 4; i++)
    {
        if (SpotUsed[i])
        {
            float3 lightDirection = normalize(SpotPosition[i].xyz - input.FragmentPos.xyz);
			
            float diffuse = max(dot(normal, lightDirection), 0.0f);
			
            float3 reflectDirection = reflect(-lightDirection, normal);
			
            float specular = pow(max(dot(viewDirection, reflectDirection), 0.0f), MaterialShininess);
			
            float distance = length(PointPosition.xyz - input.FragmentPos.xyz);
            float attenuation = 1.0f / (SpotAttenuationConstant[i] + SpotAttenuationLinear[i] * distance + SpotAttenuationQuad[i] * distance * distance);

            float theta = dot(lightDirection, normalize(-SpotDirection[i].xyz));
            float intensity = clamp((theta - SpotOuterAngle[i]) / (SpotInnerAngle[i] - SpotOuterAngle[i]), 0.0f, 1.0f);
            
            if (diffuse > 0.9f)
            {
                diffuse = 0.9f;
            }
            else if (diffuse > 0.5f)
            {
                diffuse = 0.5f;
            }
            else if (diffuse > 0.25f)
            {
                diffuse = 0.25f;
            }
            else
            {
                diffuse = 0.0f;
            }
            
            if (specular > 0.9f)
            {
                specular = 0.9f;
            }
            else if (specular > 0.5f)
            {
                specular = 0.5f;
            }
            else if (specular > 0.25f)
            {
                specular = 0.25f;
            }
            else
            {
                specular = 0.0f;
            }
			
            color += MaterialAmbient * SpotColor[i] * attenuation * intensity;
            color += MaterialDiffuse * SpotColor[i] * diffuse * attenuation * intensity;
            color += MaterialSpecular * SpotColor[i] * specular * attenuation * intensity;
        }
    }

    return float4(color.x, color.y, color.z, 1.0f);
}