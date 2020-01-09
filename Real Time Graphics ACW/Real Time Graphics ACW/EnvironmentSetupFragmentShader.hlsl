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

struct GS_OUTPUT
{
    float4 Pos : SV_Position;
    float3 FragmentPos : POSITION0;
    float4 Normal : NORMAL0;
    float4 LightFragmentPos : POSITION1;
    float3 ViewPosition : POSITION2;
    float4 Position : POSITION3;
    uint Layer : SV_RenderTargetArrayIndex;
};

float InkDirectionalFactorCalculation(float3 pFragPos, float3 pViewPosition);
float InkPointFactorCalculation(float3 pFragPos, float3 pLightPos, float3 pViewPosition);

float DirectionalShadowCalculation(float4 pPos, float4 lightPos, float3 lightDir, float3 normal);
float PointShadowCalculation(float4 pPos, float3 pFragPos, float3 pLightPos, float pFarPlane, TextureCube pTexture1, Texture2D pTexture2);

float4 main(GS_OUTPUT input) : SV_Target
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

        color += MaterialAmbient.xyz * DirectionalColor.xyz;
        color += MaterialDiffuse.xyz * DirectionalColor.xyz * diffuse;
        color += MaterialSpecular.xyz * DirectionalColor.xyz * specular;
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
        
        color += MaterialAmbient.xyz * PointColor.xyz * attenuation;
        color += MaterialDiffuse.xyz * PointColor.xyz * diffuse * attenuation;
        color += MaterialSpecular.xyz * PointColor.xyz * specular * attenuation;
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
            
            color += MaterialAmbient.xyz * SpotColor[i].xyz * attenuation *  intensity;
            color += MaterialDiffuse.xyz * SpotColor[i].xyz * diffuse * attenuation * intensity;
            color += MaterialSpecular.xyz * SpotColor[i].xyz * specular * attenuation * intensity;
        }
    }
    
    return float4(color, 1.0f);
}