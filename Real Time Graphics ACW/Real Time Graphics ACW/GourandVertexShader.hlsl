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
    matrix DirectionalView;
    matrix DirectionalProjection;
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
    float4 SpotFarPlane;
}

struct VS_INPUT
{
    float4 Pos : POSITION;
    float4 Normal : NORMAL;
};

struct VS_OUTPUT
{
    float4 Pos : SV_POSITION;
    float3 DirectionAmbientColor : COLOR0;
    float3 DirectionDiffuse : COLOR1;
    float3 DirectionSpecular : COLOR2;
    float3 PointAmbientColor : COLOR3;
    float3 PointDiffuseColor : COLOR4;
    float3 PointSpecularColor : COLOR5;
    float3 SpotAmbientColor[4] : COLOR6;
    float3 SpotDiffuseColor[4] : COLOR10;
    float3 SpotSpecularColor[4] : TANGENT0;
    float3 Normal : NORMAL0;
    float4 FragmentPos : POSITION0;
    float4 LightFragmentPos : POSITION1;
    float3 ViewPosition : POSITION2;
};

VS_OUTPUT main(VS_INPUT input)
{
    VS_OUTPUT output = (VS_OUTPUT) 0;
    output.FragmentPos = mul(input.Pos, World);
    output.LightFragmentPos = mul(output.FragmentPos, DirectionalView);
    output.LightFragmentPos = mul(output.LightFragmentPos, DirectionalProjection);
    output.Pos = mul(output.FragmentPos, View);
    output.Pos = mul(output.Pos, Projection);
    output.Normal = mul(input.Normal, InverseWorld);
    output.ViewPosition = ViewPosition;
    
    float3 viewDirection = normalize(ViewPosition - output.FragmentPos.xyz);
    float3 normal = normalize(output.Normal);

	//Directional Light
    if (DirectionalUsed)
    {
        float3 lightDirection = normalize(-DirectionalDirection);

        float diffuse = max(dot(normal, lightDirection), 0.0f);

        float3 reflectDirection = reflect(-lightDirection, normal);
		
        float specular = pow(max(dot(viewDirection, reflectDirection), 0.0f), MaterialShininess);

        output.DirectionAmbientColor = MaterialAmbient.xyz * DirectionalColor.xyz;
        output.DirectionDiffuse = MaterialDiffuse.xyz * DirectionalColor.xyz * diffuse;
        output.DirectionSpecular = MaterialSpecular.xyz * DirectionalColor.xyz * specular;
    }

	//Point Light
    if (PointUsed)
    {
        float3 lightDirection = normalize(PointPosition.xyz - output.FragmentPos.xyz);

        float diffuse = max(dot(normal, lightDirection), 0.0f);

        float3 reflectDirection = reflect(-lightDirection, normal);

        float specular = pow(max(dot(viewDirection, reflectDirection), 0.0f), MaterialShininess);

        float distance = length(PointPosition.xyz - output.FragmentPos.xyz);
        float attenuation = 1.0f / (PointAttenuationConstant + PointAttenuationLinear * distance + PointAttenuationQuad * distance * distance);

        output.PointAmbientColor = MaterialAmbient.xyz * PointColor.xyz * attenuation;
        output.PointDiffuseColor = MaterialDiffuse.xyz * PointColor.xyz * diffuse * attenuation;
        output.PointSpecularColor = MaterialSpecular.xyz * PointColor.xyz * specular * attenuation;
    }

	//Spot Light
    for (int i = 0; i < 4; i++)
    {
        if (SpotUsed[i])
        {
            float3 lightDirection = normalize(SpotPosition[i].xyz - output.FragmentPos.xyz);
			
            float diffuse = max(dot(normal, lightDirection), 0.0f);
			
            float3 reflectDirection = reflect(-lightDirection, normal);
			
            float specular = pow(max(dot(viewDirection, reflectDirection), 0.0f), MaterialShininess);
			
            float distance = length(SpotPosition[i].xyz - output.FragmentPos.xyz);
            float attenuation = 1.0f / (SpotAttenuationConstant[i] + SpotAttenuationLinear[i] * distance + SpotAttenuationQuad[i] * distance * distance);

            float theta = dot(lightDirection, normalize(SpotDirection[i].xyz));
            float intensity = clamp((theta - (1.0f - SpotOuterAngle[i])) / ((1.0f - SpotInnerAngle[i]) - (1.0f - SpotOuterAngle[i])), 0.0f, 1.0f);
            
            output.SpotAmbientColor[i] = MaterialAmbient.xyz * SpotColor[i].xyz * attenuation * intensity;
            output.SpotDiffuseColor[i] = MaterialDiffuse.xyz * SpotColor[i].xyz * diffuse * attenuation * intensity;
            output.SpotSpecularColor[i] = MaterialSpecular.xyz * SpotColor[i].xyz * specular * attenuation * intensity;
        }
    }
    
    return output;
}