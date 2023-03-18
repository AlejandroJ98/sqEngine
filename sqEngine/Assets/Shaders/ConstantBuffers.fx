
cbuffer TransformBuffer : register(b0)
{
	matrix world;
	matrix wvp;
	float3 viewPosition;
	float bumpWeight;
}

cbuffer LightBuffer : register(b1)
{
	float3 lightDirection;
	float4 lightAmbient;
	float4 lightDiffuse;
	float4 lightSpecular;
}

cbuffer MaterialBuffer : register(b2)
{
	float4 materialAmbient;
	float4 materialDiffuse;
	float4 materialSpecular;
    float4 materialEmissive;
	float materialPower;
}

cbuffer SettingBuffer : register(b3)
{
	bool useDiffuseMap;
	bool useSpecularMap;
	bool useBumpMap;
	bool useNormalMap;
}

