cbuffer GlobalBuffer : register(b10)
{
    float Time;
    int ScreenMode;
    int ShadowMode;
    float InkHeight;
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

Texture2D posTexture : register(t6);
SamplerState posSampler : register(s6);

Texture2D lightPosTexture : register(t7);
SamplerState lightPosSampler : register(s7);

Texture2D lightNormTexture : register(t8);
SamplerState lightNormSampler : register(s8);

Texture2D normalTexture : register(t9);
SamplerState normalSampler : register(s9);

Texture2D ambientTexture : register(t10);
SamplerState ambientSampler : register(s10);

Texture2D diffuseTexture : register(t11);
SamplerState diffuseSampler : register(s11);

Texture2D specularTexture : register(t12);
SamplerState specularSampler : register(s12);

Texture2D depthTexture : register(t13);
SamplerState depthSampler : register(s13);

struct VS_OUTPUT
{
    float4 Pos : SV_POSITION;
    float3 ViewPosition : POSITION0;
    float2 TexCoord : TEXCOORD0;
};

struct PS_OUTPUT
{
    float4 Color : SV_Target;
    float Depth : SV_Depth;
};

float DirectionalShadowCalculation(float4 lightPos, float3 lightDir, float3 normal);
float PointShadowCalculation(float3 pFragPos, float3 pLightPos, float pFarPlane, TextureCube pTexture, SamplerState pSampler);

PS_OUTPUT main(VS_OUTPUT input)
{
    float visable = posTexture.Sample(posSampler, input.TexCoord).a;
    
    if (visable == 0.0f)
    {
        discard;
    }
    
    PS_OUTPUT output = (PS_OUTPUT) 0;
    
    if (ScreenMode == 1)
    {
        output.Color = posTexture.Sample(posSampler, input.TexCoord);
    }
    else if (ScreenMode == 2)
    {
        output.Color = lightPosTexture.Sample(lightPosSampler, input.TexCoord);
    }
    else if (ScreenMode == 3)
    {
        output.Color = lightNormTexture.Sample(lightNormSampler, input.TexCoord);
    }
    else if (ScreenMode == 4)
    {
        output.Color = normalTexture.Sample(normalSampler, input.TexCoord);
    }
    else if (ScreenMode == 5)
    {
        output.Color = ambientTexture.Sample(ambientSampler, input.TexCoord);
    }
    else if (ScreenMode == 6)
    {
        output.Color = diffuseTexture.Sample(diffuseSampler, input.TexCoord);
    }
    else if (ScreenMode == 7)
    {
        output.Color = specularTexture.Sample(specularSampler, input.TexCoord);
    }
    else
    {
        float3 MaterialAmbient = ambientTexture.Sample(ambientSampler, input.TexCoord).rgb;
        float3 MaterialDiffuse = diffuseTexture.Sample(diffuseSampler, input.TexCoord).rgb;
        float4 tempSpecular = specularTexture.Sample(specularSampler, input.TexCoord).rgba;
        float3 MaterialSpecular = tempSpecular.rgb;
        float shininess = tempSpecular.a;
        
        float3 fragPos = posTexture.Sample(posSampler, input.TexCoord).rgb;
        float4 lightPos = lightPosTexture.Sample(lightPosSampler, input.TexCoord);
        float3 lightNorm = lightNormTexture.Sample(lightNormSampler, input.TexCoord).rgb;
        float3 normal = normalTexture.Sample(normalSampler, input.TexCoord).rgb;
        
        normal = normalize(normal);
        lightNorm = normalize(lightNorm);
        
        float3 viewDirection = normalize(input.ViewPosition - fragPos.xyz);

        float3 color = float3(0.0f, 0.0f, 0.0f);

        //Directional Light

        if (DirectionalUsed)
        {
            float3 lightDirection = normalize(-DirectionalDirection);

            float diffuse = max(dot(normal, lightDirection), 0.0f);

            float3 reflectDirection = reflect(-lightDirection, normal);
		
            float specular = pow(max(dot(viewDirection, reflectDirection), 0.0f), shininess);
        
            float shadow = DirectionalShadowCalculation(lightPos, lightDirection, lightNorm);

            color += MaterialAmbient * DirectionalColor.xyz;
            color += MaterialDiffuse * DirectionalColor.xyz * shadow * diffuse;
            color += MaterialSpecular * DirectionalColor.xyz * shadow * specular;
        }

	    //Point Light
        if (PointUsed)
        {
            float3 lightDirection = normalize(PointPosition.xyz - fragPos.xyz);

            float diffuse = max(dot(normal, lightDirection), 0.0f);

            float3 reflectDirection = reflect(-lightDirection, normal);

            float specular = pow(max(dot(viewDirection, reflectDirection), 0.0f), shininess);

            float distance = length(PointPosition.xyz - fragPos.xyz);
            float attenuation = 1.0f / (PointAttenuationConstant + PointAttenuationLinear * distance + PointAttenuationQuad * distance * distance);
        
            float shadow = PointShadowCalculation(fragPos.xyz, PointPosition.xyz, PointFarPlane, pointShadowTexture, pointShadowSampler);

            color += MaterialAmbient * PointColor.xyz * attenuation;
            color += MaterialDiffuse * PointColor.xyz * diffuse * shadow * attenuation;
            color += MaterialSpecular * PointColor.xyz * specular * shadow * attenuation;
        }

	    //Spot Light
        for (int i = 0; i < 4; i++)
        {
            if (SpotUsed[i])
            {
                float3 lightDirection = normalize(SpotPosition[i].xyz - fragPos.xyz);
			
                float diffuse = max(dot(normal, lightDirection), 0.0f);
			
                float3 reflectDirection = reflect(-lightDirection, normal);
			
                float specular = pow(max(dot(viewDirection, reflectDirection), 0.0f), shininess);
			
                float distance = length(SpotPosition[i].xyz - fragPos.xyz);
                float attenuation = 1.0f / (SpotAttenuationConstant[i] + SpotAttenuationLinear[i] * distance + SpotAttenuationQuad[i] * distance * distance);

                float theta = dot(lightDirection, normalize(SpotDirection[i].xyz));
            
                float intensity = clamp((theta - (1.0f - SpotOuterAngle[i])) / ((1.0f - SpotInnerAngle[i]) - (1.0f - SpotOuterAngle[i])), 0.0f, 1.0f);
            
                float shadow;
            
                if (i == 0)
                {
                    shadow = PointShadowCalculation(fragPos.xyz, SpotPosition[i].xyz, SpotFarPlane[i], spot1ShadowTexture, spot1ShadowSampler);
                }
                else if (i == 1)
                {
                    shadow = PointShadowCalculation(fragPos.xyz, SpotPosition[i].xyz, SpotFarPlane[i], spot2ShadowTexture, spot2ShadowSampler);
                }
                else if (i == 2)
                {
                    shadow = PointShadowCalculation(fragPos.xyz, SpotPosition[i].xyz, SpotFarPlane[i], spot3ShadowTexture, spot3ShadowSampler);
                }
                else if (i == 3)
                {
                    shadow = PointShadowCalculation(fragPos.xyz, SpotPosition[i].xyz, SpotFarPlane[i], spot4ShadowTexture, spot4ShadowSampler);
                }
			
                color += MaterialAmbient * SpotColor[i].xyz * attenuation * shadow * intensity;
                color += MaterialDiffuse * SpotColor[i].xyz * diffuse * attenuation * shadow * intensity;
                color += MaterialSpecular * SpotColor[i].xyz * specular * attenuation * shadow * intensity;
            }
        }
    
        output.Color = float4(color, 1.0f);
    }
    
    output.Depth = depthTexture.Sample(depthSampler, input.TexCoord).r;
    
    return output;
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
    
    int shadowType = 0;
    
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
    
    int shadowType = 0;
    
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