
#include "SSR.hlsli"

// Texture
Texture2D diffuseTexture : register(t0);
Texture2D positionTexture : register(t1);
Texture2D normalTexture : register(t2);
Texture2D depthTexture : register(t3);

SamplerState SampleType : register(s0);

cbuffer PerFrameBuffer: register(b1)
{
	float4x4 VM;
	float4x4 VM_Inv;
	float4x4 PM;
	float4x4 PM_Inv;
	float4 CameraPosition;
};

struct VOut
{
	float4 position : SV_POSITION;
	float4 normal : NORMAL;
	float2 texcoord : TEXCOORD;
};

float4 main(VOut IN) : SV_TARGET
{
	float4 textureColour = diffuseTexture.Sample(SampleType, IN.texcoord);
	float4 world_position = positionTexture.Sample(SampleType, IN.texcoord); // world space
	float4 normal = normalTexture.Sample(SampleType, IN.texcoord); // world space
	float depth = depthTexture.Sample(SampleType, IN.texcoord).r;

	float4 view_ray = normal - CameraPosition;
	float4 view_dir = normalize(view_ray);



	float4 output = view_dir;
	return output;
}