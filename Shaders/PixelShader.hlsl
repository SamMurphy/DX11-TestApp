
// Texture
Texture2D shaderTexture;
SamplerState SampleType;

struct VOut
{
	float4 position : SV_POSITION;
	float4 normal : NORMAL;
	float2 texcoord : TEXCOORD;
};

float4 main(VOut IN) : SV_TARGET
{
	float4 textureColour = shaderTexture.Sample(SampleType, IN.texcoord);
	return textureColour;
}