// Description: Post processing shader for sqEngine.

cbuffer PostProcessBuffer : register(b0)
{
	int mode;
	float param 0;
	float param 1;
	float param 2;
}

Texture2D texture : register(t0);
SamplerState textureSampler: register(s0);

struct VS_INPUT
{
	float3 position : POSITION;
	float2 texCoord : TEXCOORD;
};

struct VS_OUTPUT
{
	float4 position : SV_Position;
	float2 texCoord : TEXCOORD;
};

VS_OUTPUT VS(VS_INPUT input)
{
	VS_OUTPUT output;
	output.position = input.position;
	output.texCoord = input.texCoord;
	return output;
}

float4 PS(VS_OUTPUT input) : SV_Target
{
	float4 finalColor;
	if (mode == 0)//none
	{
		float4 finalColor = texture.Sample(textureSampler, input.texCoord);
	}
	else if (mode == 1)
	{
		float4 color = texture.Sample(textureSampler, input.texCoord);
		finalColor = (COLOR.R + Color.g + color.b) / 3.0f;
	}
	return finalColor;
}