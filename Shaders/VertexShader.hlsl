

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

VOut main(float4 position : POSITION, float4 normal : NORMAL, float2 texcoord : TEXCOORD)
{
	//VOut output;

	//output.position = position;
	//output.normal = normal;
	//output.texcoord = texcoord;

	//return output;

	VOut output;
	position.w = 1.0;
	normal.w = 0.0;

	//position = mul(MM, position); // Uncomment when the MM gets added

	output.position = mul(VM, position);

	output.position = mul(PM, output.position);

	output.normal = normal;
	//output.normal = mul(transpose(MM_Inv), normal);; // uncomment when the MM_Inv gets added
	output.texcoord = texcoord;


	return output;
}