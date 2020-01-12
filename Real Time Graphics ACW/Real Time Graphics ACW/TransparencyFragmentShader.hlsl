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

cbuffer GlobalBuffer : register(b10)
{
    float Time;
    int ScreenMode;
    int ShadowMode;
    float InkHeight;
}

Texture2D directionalShadowTexture : register(t0);
SamplerState Sampler : register(s0);

TextureCube pointShadowTexture : register(t1);

TextureCube spot1ShadowTexture : register(t2);
TextureCube spot2ShadowTexture : register(t3);
TextureCube spot3ShadowTexture : register(t4);
TextureCube spot4ShadowTexture : register(t5);

Texture2D directionalSimpleShadowTexture : register(t6);

Texture2D pointSimpleShadowTexture : register(t7);

Texture2D spot1SimpleShadowTexture : register(t8);
Texture2D spot2SimpleShadowTexture : register(t9);
Texture2D spot3SimpleShadowTexture : register(t10);
Texture2D spot4SimpleShadowTexture : register(t11);

struct VS_OUTPUT
{
    float4 Pos : SV_POSITION;
    float4 FragmentPos : POSITION0;
    float4 Normal : NORMAL0;
    float3 ViewPosition : POSITION1;
    float4 LightFragmentPos : POSITION2;
    float4 Position : POSITION3;
};

float InkDirectionalFactorCalculation(float3 pFragPos, float3 pViewPosition);
float InkPointFactorCalculation(float3 pFragPos, float3 pLightPos, float3 pViewPosition);

float DirectionalShadowCalculation(float4 pPos, float4 lightPos, float3 lightDir, float3 normal);
float PointShadowCalculation(float4 pPos, float3 pFragPos, float3 pLightPos, float pFarPlane, TextureCube pTexture1, Texture2D pTexture2);

float4 main(VS_OUTPUT input) : SV_Target
{
    float3 normal = normalize(input.Normal.xyz);
    float3 viewDirection = normalize(input.ViewPosition - input.FragmentPos.xyz);

    float3 color = float3(0.0f, 0.0f, 0.0f);
    float3 inkColor = float3(0.0f, 0.0f, 1.0f);

	//Directional Light
    if (DirectionalUsed)
    {
        float3 lightDirection = normalize(-DirectionalDirection);

        float diffuse = max(dot(normal, lightDirection), 0.0f);

        float3 reflectDirection = reflect(-lightDirection, normal);
		
        float specular = pow(max(dot(viewDirection, reflectDirection), 0.0f), MaterialShininess);
        
        float shadow = DirectionalShadowCalculation(input.Position, input.LightFragmentPos, lightDirection, input.Normal.xyz);
        
        float inkFactor = InkDirectionalFactorCalculation(input.FragmentPos.xyz, input.ViewPosition);

        color += lerp(MaterialAmbient.xyz, inkColor * 0.1f, inkFactor) * DirectionalColor.xyz;
        color += lerp(MaterialDiffuse.xyz, inkColor, inkFactor) * DirectionalColor.xyz * shadow * diffuse;
        color += lerp(MaterialSpecular.xyz, inkColor, inkFactor) * DirectionalColor.xyz * shadow * specular;
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

        float shadow = PointShadowCalculation(input.Position, input.FragmentPos.xyz, PointPosition, PointFarPlane, pointShadowTexture, pointSimpleShadowTexture);
        
        float inkFactor = InkPointFactorCalculation(input.FragmentPos.xyz, PointPosition.xyz, input.ViewPosition);
        
        color += lerp(MaterialAmbient.xyz, inkColor * 0.1f, inkFactor) * PointColor.xyz * attenuation;
        color += lerp(MaterialDiffuse.xyz, inkColor, inkFactor) * PointColor.xyz * diffuse * shadow * attenuation;
        color += lerp(MaterialSpecular.xyz, inkColor, inkFactor) * PointColor.xyz * specular * shadow * attenuation;
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
                shadow = PointShadowCalculation(input.Position, input.FragmentPos.xyz, SpotPosition[i].xyz, SpotFarPlane[i], spot1ShadowTexture, spot1SimpleShadowTexture);
            }
            else if (i == 1)
            {
                shadow = PointShadowCalculation(input.Position, input.FragmentPos.xyz, SpotPosition[i].xyz, SpotFarPlane[i], spot2ShadowTexture, spot2SimpleShadowTexture);
            }
            else if (i == 2)
            {
                shadow = PointShadowCalculation(input.Position, input.FragmentPos.xyz, SpotPosition[i].xyz, SpotFarPlane[i], spot3ShadowTexture, spot3SimpleShadowTexture);
            }
            else if (i == 3)
            {
                shadow = PointShadowCalculation(input.Position, input.FragmentPos.xyz, SpotPosition[i].xyz, SpotFarPlane[i], spot4ShadowTexture, spot4SimpleShadowTexture);
            }
            
            float inkFactor = InkPointFactorCalculation(input.FragmentPos.xyz, SpotPosition[i].xyz, input.ViewPosition);
            
            color += lerp(MaterialAmbient.xyz, inkColor * 0.1f, inkFactor) * SpotColor[i].xyz * attenuation * shadow * intensity;
            color += lerp(MaterialDiffuse.xyz, inkColor, inkFactor) * SpotColor[i].xyz * diffuse * attenuation * shadow * intensity;
            color += lerp(MaterialSpecular.xyz, inkColor, inkFactor) * SpotColor[i].xyz * specular * attenuation * shadow * intensity;
        }
    }
    
    return float4(color, 0.5f);
}

float InkDirectionalFactorCalculation(float3 pFragPos, float3 pViewPosition)
{
    float fogInk = 1.0f;
    
    if (pFragPos.y > InkHeight)
    {
        return 0.0f;
    }
  
    float fogCamera = (20.0f - length(pFragPos - pViewPosition)) / (20.0f - 5.0f);
    
    fogCamera = clamp(fogCamera, 0.0f, 0.5f);
    
    fogCamera = 1.0f - fogCamera;
    
    return fogInk * fogCamera;
}

float InkPointFactorCalculation(float3 pFragPos, float3 pLightPos, float3 pViewPosition)
{
    float fogInk = 1.0f;
    
    if (pFragPos.y > InkHeight)
    {
        return 0.0f;
    }
  
    float fogCamera = (20.0f - length(pFragPos - pViewPosition)) / (20.0f - 5.0f);
    
    fogCamera = clamp(fogCamera, 0.0f, 0.5f);
    
    fogCamera = 1.0f - fogCamera;
    
    return fogInk * fogCamera;
}

float DirectionalShadowCalculation(float4 pPos, float4 lightPos, float3 lightDir, float3 normal)
{
    float shadow = 0.0f;
    
    if (ShadowMode == 0)
    {
        float3 projCoords = pPos.xyz / pPos.w;
    
        projCoords.y = -projCoords.y;
    
        projCoords.xy = projCoords.xy * 0.5f + 0.5f;
    
        if (projCoords.x < 0.0f || projCoords.y < 0.0f || projCoords.x > 1.0f || projCoords.y > 1.0f)
        {
            return 1.0f;
        }
        
        float currentDepth = projCoords.z;
        float closestDepth = directionalSimpleShadowTexture.Sample(Sampler, projCoords.xy).r;
        
        shadow = currentDepth > closestDepth ? 1.0f : 0.0f;
        return (1.0 - shadow);
    }
    
    float3 projCoords = lightPos.xyz / lightPos.w;
    
    projCoords.y = -projCoords.y;
    
    projCoords.xy = projCoords.xy * 0.5f + 0.5f;
    
    if (projCoords.x < 0.0f || projCoords.y < 0.0f || projCoords.x > 1.0f || projCoords.y > 1.0f)
    {
        return 1.0f;
    }
    
    if (projCoords.z > 1.0)
    {
        return 1.0f;
    }
    
    float bias = max(0.05f * (1.0f - dot(normal, lightDir)), 0.005);
    float currentDepth = projCoords.z;
    
    if (ShadowMode == 1) //No PCF or VSM
    {
        float closestDepth = directionalShadowTexture.Sample(Sampler, projCoords.xy).r;
        
        shadow = currentDepth - bias > closestDepth ? 1.0f : 0.0f;
    }
    else if (ShadowMode == 2) //PCF
    {
        float2 texSize = float2(1.0f / 1024.0f, 1.0f / 1024.0f);
        
        for (int x = -2; x <= 2; ++x)
        {
            for (int y = -2; y <= 2; ++y)
            {
                float closestDepth = directionalShadowTexture.Sample(Sampler, projCoords.xy + float2(x, y) * texSize).r;
                shadow += currentDepth - bias > closestDepth ? 1.0f : 0.0f;
            }
        }
        shadow /= 25.0f;
    }
    else if (ShadowMode == 3) //VSM
    {
        currentDepth = currentDepth - bias;
        float2 texSize = float2(1.0f / 1024.0f, 1.0f / 1024.0f);
        
        for (int x = -2; x <= 2; ++x)
        {
            for (int y = -2; y <= 2; ++y)
            {
        
    //Based on Source: http://developer.download.nvidia.com/SDK/10/direct3d/Source/VarianceShadowMapping/Doc/VarianceShadowMapping.pdf
                float2 moments = directionalShadowTexture.Sample(Sampler, projCoords.xy + float2(x, y) * texSize).rg;
        
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

float PointShadowCalculation(float4 pPos, float3 pFragPos, float3 pLightPos, float pFarPlane, TextureCube pTexture1, Texture2D pTexture2)
{
    float shadow = 0.0f;
    
    if (ShadowMode == 0)
    {
        float3 projCoords = pPos.xyz / pPos.w;
    
        projCoords.y = -projCoords.y;
    
        projCoords.xy = projCoords.xy * 0.5f + 0.5f;
    
        if (projCoords.x < 0.0f || projCoords.y < 0.0f || projCoords.x > 1.0f || projCoords.y > 1.0f)
        {
            return 1.0f;
        }
    
        if (projCoords.z > 1.0)
        {
            return 1.0f;
        }
        
        float currentDepth = projCoords.z;
        
        float closestDepth = pTexture2.Sample(Sampler, projCoords.xy).r;
        
        shadow = currentDepth > closestDepth ? 1.0f : 0.0f;
        
        return (1.0 - shadow);
    }
    
    
    float3 vec = pFragPos - pLightPos;
    
    float currentDepth = length(vec);
    
    float bias = 0.05f;
    
    if (ShadowMode == 1) //No PCF or VSM
    {
        float closestDepth = pTexture1.Sample(Sampler, vec).r;
        closestDepth *= pFarPlane;
        
        shadow = currentDepth - bias > closestDepth ? 1.0f : 0.0f;
    }
    else if (ShadowMode == 2) //PCF
    {
        float samples = 4.0f;
        float offset = 0.01f;
        
        for (float x = -offset; x < offset; x += offset / (samples * 0.5))
        {
            for (float y = -offset; y < offset; y += offset / (samples * 0.5))
            {
                for (float z = -offset; z < offset; z += offset / (samples * 0.5))
                {
                    float closestDepth = pTexture1.Sample(Sampler, vec + float3(x, y, z)).r;
                    closestDepth *= pFarPlane;
                  
                    shadow += currentDepth - bias > closestDepth ? 1.0f : 0.0f;
                }
            }
        }
        
        shadow /= (samples * samples * samples);
    }
    else if (ShadowMode == 3)//VSM
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
                    float2 moments = pTexture1.Sample(Sampler, vec + float3(x, y, z)).rg;
        
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