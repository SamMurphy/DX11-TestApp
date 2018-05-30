
// Texture
Texture2D shaderTexture;
SamplerState SampleType;

float3 CalcLuma(float3 colour)
{
    return max(dot(colour, float3(0.299f, 0.587f, 0.114f)), 0.0001f);
}

struct VOut
{
	float4 position : SV_POSITION;
	float2 texcoord : TEXCOORD;
};

float4 main(VOut IN) : SV_TARGET
{
	float4 textureColour = shaderTexture.Sample(SampleType, IN.texcoord);

    textureColour.rgb = CalcLuma(textureColour.rgb);

	return textureColour;
}