#include "Particle.hlsli"
static const int vnum = 4;

static const float4 offset_array[vnum] =
{
	float4(-0.5f, -0.5f, 0, 0),	// ¶‰º
	float4(-0.5f, +0.5f, 0, 0),	// ¶ã
	float4(+0.5f, -0.5f, 0, 0),	// ‰Eã
	float4(+0.5f, +0.5f, 0, 0),	// ‰E‰º

};

[maxvertexcount(vnum)]
void main(
	point PS_INPUT input[1],
	inout TriangleStream< PS_INPUT > output
)
{
	for (uint i = 0; i < vnum; i++)
	{
		PS_INPUT element;

		element.Pos = input[0].Pos + offset_array[i] * 2;

		//element.Pos = mul(element.Pos, matWorld);
		//element.Pos = mul(element.Pos, matView);
		//element.Pos = mul(element.Pos, matProj);

		float aspect = 600.0f / 800.0f;

		element.Color = input[0].Color;
		
		//element.Tex = -offset_array[i]+float2(0.5f,0.5f);
		element.Tex = float2(offset_array[i].x , -offset_array[i].y) + float2(0.5f ,0.5f);

		//element.Tex /= aspect;

		output.Append(element);
	}
	output.RestartStrip();
}