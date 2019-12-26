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
    matrix DirectionalView;
    matrix DirectionalProjection;
}

cbuffer pointBuffer : register(b3)
{
    float4 PointColor;
    float3 PointPosition;
    float PointAttenuationConstant;
    float PointAttenuationLinear;
    float PointAttenuationQuad;
    int PointUsed;
    float PointFarPlane;
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
    float4 SpotFarPlane;
}

Texture2D directionalShadowTexture : register(t0);
SamplerState directionalShadowSampler : register(s0);

TextureCube pointShadowTexture : register(t1);
SamplerState pointShadowSampler : register(s1);

TextureCube spot1ShadowTexture : register(t2);
SamplerState spot1ShadowSampler : register(s2);

TextureCube spot2ShadowTexture : register(t3);
SamplerState spot2ShadowSampler : register(s3);

TextureCube spot3ShadowTexture : register(t4);
SamplerState spot3ShadowSampler : register(s4);

TextureCube spot4ShadowTexture : register(t5);
SamplerState spot4ShadowSampler : register(s5);

struct VS_OUTPUT
{
    float4 Pos : SV_POSITION;
    float4 FragmentPos : POSITION0;
    float4 Normal : NORMAL0;
    float3 ViewPosition : POSITION1;
    float4 LightFragmentPos : POSITION2;
};

float DirectionalShadowCalculation(float4 lightPos, float3 lightDir, float3 normal);
float PointShadowCalculation(float3 pFragPos, float3 pLightPos, float pFarPlane, TextureCube pTexture, SamplerState pSampler);

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
        
        float shadow = DirectionalShadowCalculation(input.LightFragmentPos, lightDirection, input.Normal.xyz);

        color += MaterialAmbient * DirectionalColor;
        color += MaterialDiffuse * DirectionalColor * shadow * diffuse;
        color += MaterialSpecular * DirectionalColor * shadow * specular;
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
        
        float shadow = PointShadowCalculation(input.FragmentPos.xyz, PointPosition.xyz, PointFarPlane, pointShadowTexture, pointShadowSampler);

        color += MaterialAmbient * PointColor * attenuation;
        color += MaterialDiffuse * PointColor * diffuse * shadow * attenuation;
        color += MaterialSpecular * PointColor * specular * shadow * attenuation;
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
			
            float distance = length(SpotPosition[i].xyz - input.FragmentPos.xyz);
            float attenuation = 1.0f / (SpotAttenuationConstant[i] + SpotAttenuationLinear[i] * distance + SpotAttenuationQuad[i] * distance * distance);

            float theta = dot(lightDirection, normalize(SpotDirection[i].xyz));
            
            float intensity = clamp((theta - (1.0f - SpotOuterAngle[i])) / ((1.0f - SpotInnerAngle[i]) - (1.0f - SpotOuterAngle[i])), 0.0f, 1.0f);
            
            float shadow;
            
            if (i == 0)
            {
                shadow = PointShadowCalculation(input.FragmentPos.xyz, SpotPosition[i].xyz, SpotFarPlane[i], spot1ShadowTexture, spot1ShadowSampler);
            }
            else if (i == 1)
            {
                shadow = PointShadowCalculation(input.FragmentPos.xyz, SpotPosition[i].xyz, SpotFarPlane[i], spot2ShadowTexture, spot2ShadowSampler);
            }
            else if (i == 2)
            {
                shadow = PointShadowCalculation(input.FragmentPos.xyz, SpotPosition[i].xyz, SpotFarPlane[i], spot3ShadowTexture, spot3ShadowSampler);
            }
            else if (i == 3)
            {
                shadow = PointShadowCalculation(input.FragmentPos.xyz, SpotPosition[i].xyz, SpotFarPlane[i], spot4ShadowTexture, spot4ShadowSampler);
            }
			
            color += MaterialAmbient * SpotColor[i] * attenuation * shadow * intensity;
            color += MaterialDiffuse * SpotColor[i] * diffuse * attenuation * shadow * intensity;
            color += MaterialSpecular * SpotColor[i] * specular * attenuation * shadow * intensity;
        }
    }
    
    return float4(color, 1.0f);
}

float DirectionalShadowCalculation(float4 lightPos, float3 lightDir, float3 normal)
{
    float3 projCoords = lightPos.xyz / lightPos.w;
    
    projCoords.y = -projCoords.y;
    
    projCoords.xy = projCoords.xy * 0.5f + 0.5f;
    
    if (projCoords.z > 1.0)
    {
        return 1.0f;
    }
    
    float bias = max(0.05f * (1.0f - dot(normal, lightDir)), 0.005);
    float currentDepth = projCoords.z;
    
    float shadow = 0.0f;
    
    int shadowType = 2;
    
    if (shadowType == 0) //No PCF or VSM
    {
        float closestDepth = directionalShadowTexture.Sample(directionalShadowSampler, projCoords.xy).r;
        
        shadow = currentDepth - bias > closestDepth ? 1.0f : 0.0f;
    }
    else if (shadowType == 1) //PCF
    {
        float2 texSize = float2(1.0f / 1024.0f, 1.0f / 1024.0f);
        
        for (int x = -2; x <= 2; ++x)
        {
            for (int y = -2; y <= 2; ++y)
            {
                float closestDepth = directionalShadowTexture.Sample(directionalShadowSampler, projCoords.xy + float2(x, y) * texSize).r;
                shadow += currentDepth - bias > closestDepth ? 1.0f : 0.0f;
            }
        }
        shadow /= 25.0f;
    }
    else //VSM
    {
        currentDepth = currentDepth - bias;
        float2 texSize = float2(1.0f / 1024.0f, 1.0f / 1024.0f);
        
        for (int x = -2; x <= 2; ++x)
        {
            for (int y = -2; y <= 2; ++y)
            {
        
                //TODO: Source: http://developer.download.nvidia.com/SDK/10/direct3d/Source/VarianceShadowMapping/Doc/VarianceShadowMapping.pdf
                float2 moments = directionalShadowTexture.Sample(directionalShadowSampler, projCoords.xy + float2(x, y) * texSize).rg;
        
                float variance = moments.y - (moments.x * moments.x);

                float d = currentDepth - moments.x;
                float shadowVSM = (variance / (variance + d * d));
        
                shadowVSM = max(shadowVSM, currentDepth <= moments.x);
        
                shadow += 1.0f - shadowVSM;
            }
        }
        
        shadow /= 25.0f;
    }
    
    
    return (1.0f - shadow);
}

float PointShadowCalculation(float3 pFragPos, float3 pLightPos, float pFarPlane, TextureCube pTexture, SamplerState pSampler)
{
    float3 vec = pFragPos - pLightPos;
    
    float currentDepth = length(vec);
    
    float bias = 0.05f;
    
    float shadow = 0.0f;
    
    int shadowType = 2;
    
    if (shadowType == 0) //No PCF or VSM
    {
        float closestDepth = pTexture.Sample(pSampler, vec).r;
        closestDepth *= pFarPlane;
        
        shadow = currentDepth - bias > closestDepth ? 1.0f : 0.0f;
    }
    else if (shadowType == 1) //PCF
    {
        float samples = 4.0f;
        float offset = 0.01f;
        
        for (float x = -offset; x < offset; x += offset / (samples * 0.5))
        {
            for (float y = -offset; y < offset; y += offset / (samples * 0.5))
            {
                for (float z = -offset; z < offset; z += offset / (samples * 0.5))
                {
                    float closestDepth = pTexture.Sample(pSampler, vec + float3(x, y, z)).r;
                    closestDepth *= pFarPlane;
                  
                    shadow += currentDepth - bias > closestDepth ? 1.0f : 0.0f;
                }
            }
        }
        
        shadow /= (samples * samples * samples);
    }
    else //VSM
    {
        currentDepth /= pFarPlane;
        float samples = 4.0f;
        float offset = 0.01f;
        
        for (float x = -offset; x < offset; x += offset / (samples * 0.5))
        {
            for (float y = -offset; y < offset; y += offset / (samples * 0.5))
            {
                for (float z = -offset; z < offset; z += offset / (samples * 0.5))
                {
                    //TODO: Source: http://developer.download.nvidia.com/SDK/10/direct3d/Source/VarianceShadowMapping/Doc/VarianceShadowMapping.pdf
                    float2 moments = pTexture.Sample(pSampler, vec + float3(x, y, z)).rg;
        
                    float variance = moments.y - (moments.x * moments.x);

                    float d = currentDepth - moments.x;
                    float shadowVSM = (variance / (variance + d * d));
        
                    shadowVSM = max(shadowVSM, currentDepth <= moments.x);
        
                    shadow += 1.0f - shadowVSM;
                }
            }
        }
        
        shadow /= (samples * samples * samples);
    }

    return (1.0 - shadow);
}