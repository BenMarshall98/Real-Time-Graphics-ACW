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
};

float DirectionalShadowCalculation(float4 lightPos, float3 lightDir, float3 normal);
float PointShadowCalculation(float3 pFragPos, float3 pLightPos, float pFarPlane, TextureCube pTexture, SamplerState pSampler);

float4 main(VS_OUTPUT input) : SV_Target
{
    float3 color = float3(0.0f, 0.0f, 0.0f);
    
    //Directional Light
    if (DirectionalUsed)
    {
        float3 lightDirection = normalize(-DirectionalDirection);
        
        float shadow = DirectionalShadowCalculation(input.LightFragmentPos, lightDirection, normalize(input.Normal.xyz));
        
        color += input.DirectionAmbientColor;
        color += input.DirectionDiffuse * shadow;
        color += input.DirectionSpecular * shadow;
    }
    
    //Point Light
    if (PointUsed)
    {
        float shadow = PointShadowCalculation(input.FragmentPos.xyz, PointPosition.xyz, PointFarPlane, pointShadowTexture, pointShadowSampler);
        
        color += input.PointAmbientColor;
        color += input.PointDiffuseColor * shadow;
        color += input.PointSpecularColor * shadow;
    }
    
    //Spot Light
    for (int i = 0; i < 4; i++)
    {
        if (SpotUsed[i])
        {
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
            
            color += input.SpotAmbientColor[i];
            color += input.SpotDiffuseColor[i] * shadow;
            color += input.SpotSpecularColor[i] * shadow;
        }
    }
   
    return float4(color, 1.0f);
}

float DirectionalShadowCalculation(float4 lightPos, float3 lightDir, float3 normal)
{
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
                float2 moments = directionalShadowTexture.SampleLevel(directionalShadowSampler, projCoords.xy + float2(x, y) * texSize, 0).rg;
        
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
    
    int shadowType = 1;
    
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
        float offset = 0.001f;
        
        for (float x = -offset; x < offset; x += offset / (samples * 0.5))
        {
            for (float y = -offset; y < offset; y += offset / (samples * 0.5))
            {
                for (float z = -offset; z < offset; z += offset / (samples * 0.5))
                {
                    //TODO: Source: http://developer.download.nvidia.com/SDK/10/direct3d/Source/VarianceShadowMapping/Doc/VarianceShadowMapping.pdf
                    float2 moments = pTexture.SampleLevel(pSampler, vec + float3(x, y, z), 0).rg;
        
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