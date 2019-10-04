
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

	float4 view_ray = world_position - CameraPosition;
	float4 view_dir = normalize(view_ray);

	float4 view_dir_camera_space = mul(VM, view_dir); // Transform direction into camera space.
	view_dir_camera_space = normalize(view_dir_camera_space);

	float4 ray_origin = world_position; // Origin of the ray is the world position of the pixel.
	float4 ray_origin_camera_space = mul(VM, ray_origin); // SSR shader wants the origin in camera space, so multiple by the view matrix.

	float4 normal_camera_space = normalize(mul(VM, normal)); // Transform normal into camera space.

	float4 ray_dir_camera_space = normalize(reflect(view_dir_camera_space, normal_camera_space)); // Reflect the view direction about the normal all in camera space.

	float2 hitPixel = float2(0, 0);
	float3 hitPoint = float3(0, 0, 0);
	bool hit = traceScreenSpaceRay1(
		ray_origin_camera_space.xyz,
		ray_dir_camera_space.xyz,
		PM,
		depthTexture,
		float2(1920.0, 1080.0), // depth buffer dimensions
		0.1, // thickness
		NEAR, // near plane
		1.0, // stride, int >= 1
		0.5, // jitter 0-1
		10000.0, // max steps
		10000.0, // max distance, camera space
		hitPixel,
		hitPoint
	);

	float4 output = float4(0, 0, 0, 0);
	if (hit == true)
	{
		output = diffuseTexture.Load(int3(hitPixel, 0));
		output.a = 1.0;
		output.xy = hitPixel;
		output.z = 0.0;
	}

	//output = normal_camera_space;
	output = ray_origin_camera_space;
	//output = ray_dir_camera_space;
	//output = view_dir_camera_space;
	return output;

	//float linear_depth = lineariseDepth(depth, 0.1, 10000);
	//return linear_depth.rrrr;
}