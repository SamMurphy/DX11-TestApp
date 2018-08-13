
// Texture
Texture2D diffuseTexture;
Texture2D specularTexture;
SamplerState SampleType;

struct VOut
{
	float4 position : SV_POSITION;
	float4 normal : NORMAL;
	float2 texcoord : TEXCOORD0;
	float4 worldPos : TEXCOORD1;
};

struct PSOut
{
	float4 Position : SV_Target0;
	float4 Normal   : SV_Target1;
	float4 Diffuse  : SV_Target2;
	float4 Specular : SV_Target3;
};


PSOut main(VOut IN) : SV_TARGET
{
	PSOut output;

	float4 textureColour = diffuseTexture.Sample(SampleType, IN.texcoord);
	clip(textureColour.a - 0.4999f);

	float4 specularColour = specularTexture.Sample(SampleType, IN.texcoord);


	output.Position = IN.worldPos;
	output.Normal = IN.normal;
	output.Normal.a = 1.0f;
	output.Diffuse = textureColour;
	output.Specular = specularColour;
	output.Specular.a = 1.0f;

	return output;
}