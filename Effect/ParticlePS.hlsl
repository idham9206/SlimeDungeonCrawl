#include "Particle.hlsli"


Texture2D tex : register(t0);
Texture2D tex2 : register(t1);
SamplerState samLinear : register(s0);


//float4 main(PS_INPUT input) : SV_TARGET
//{
//	//float4 diff = tex.Sample(samLinear, input.Tex);
//	//float4 diff2 = tex.Sample(samLinear, input.Tex);
//	////diff = float4(input.Color.x,1,input.Color.z, diff.a);
//
//	//float4 base = tex.Sample(samLinear, input.Tex);
//	////return diff;
//
//	//float d = distance(float2(0.0f, 0.0f), input.Tex);
//	//float d2 = distance(float2(0.5f, 0.5f), input.Tex);
//
//	//if (d > diffuse.x)
//	//{
//	//	base.a = diffuse.x;
//	//}
//	//if (d2 > diffuse.x)
//	//{
//	//	base.a = 0;
//	//}
//
//	//return base;
//	////return float4(1,1,1,1);
//
//
//}

float random(float2 p)
{
	return frac(sin(dot(p, float2(12.9898f, 78.233f)))* 43758.5453f);
}

float noise(float2 st)
{
	float2 p = floor(st);
	return random(p);
}

float2 random2(float2 st)
{
	st = float2(dot(st, float2(127.1f, 311.7f)), dot(st, float2(269.5f, 183.3f)));

	return -1.0f + 2.0f * frac(sin(st)*43758.5453123f);
}

float perlinNoise(float2 st)
{
	float2 p = floor(st);
	float2 f = frac(st);
	float2 u = f * f * (3.0f - 2.0f * f);

	float2 v00 = random2(p + float2(0, 0));
	float2 v10 = random2(p + float2(1, 0));
	float2 v01 = random2(p + float2(0, 1));
	float2 v11 = random2(p + float2(1, 1));

	return lerp(lerp(dot(v00, f - float2(0, 0)), dot(v10, f - float2(1, 0)), u.x),
		lerp(dot(v01, f - float2(0, 1)), dot(v11, f - float2(1, 1)), u.x), u.y) + 0.5f;

}

float fbm(float2 st)
{
	float f = 0;
	float2 q = st;

	f += 0.5000f * perlinNoise(q); q = q* 2.01f;
	//f += 0.2500f * perlinNoise(q); q = q* 2.02f;
	f += 0.1250f * perlinNoise(q); q = q* 2.03f;
	f += 0.0625f * perlinNoise(q); q = q* 2.01f;

	return f;
}

//float4 main(PS_INPUT input) : SV_TARGET
//{
//
//	float2 center = float2(0.7f, 0.2f);
//	float dist = distance(center, input.Tex);
//	float r = diffuse.y;
//	float distortion = 1 - smoothstep(r - 0.07f, r, dist);
//	float2 uv = input.Tex /*+ (center - input.Tex) * distortion*/;
//
//	//float2 center2 = float2(0.2f, 0.7f);
//	//float dist2 = distance(center2, uv);
//	//float r2 = 0.7f;
//	//float distortion2 = 1 - smoothstep(r - 0.07f, r, dist2);
//	//float2 uv2 = uv + (center2 - uv) * distortion2;
//
//	//
//	float4 base = tex.Sample(samLinear, uv);
//	//float4 base2 = tex.Sample(samLinear, uv2);
//
//	float2 gateUV = input.Tex;
//	float ran = perlinNoise(input.Tex * 20);
//	gateUV.y += (ran - 0.5f) * 0.1f + diffuse.x;
//
//	float4 gate = tex2.Sample(samLinear, input.Tex);
//
//	//gate.b += 1.0f;
//
//
//	//float4 L1 = lerp(base, gate, step(1, distortion));
//	//float4 L2 = lerp(base2, gate, step(1, distortion2));
//
//	return lerp(base, gate, step(1, distortion));
//	//return tex.Sample(samLinear, input.Tex);
//}

//float4 main(PS_INPUT input) : SV_TARGET
//{
//	if (diffuse.y < 1.3f) 
//	{
//		float2 center = float2(0.7f, 0.2f);
//		float dist = distance(center, input.Tex);
//		float r = diffuse.y;
//		float distortion = 1 - smoothstep(r - 0.07f, r, dist);
//		float2 uv = input.Tex /*+ (center - input.Tex) * distortion*/;
//
//
//		float4 base = tex.Sample(samLinear, uv);
//
//		float2 gateUV = input.Tex;
//		float ran = perlinNoise(input.Tex * 20);
//		//gateUV.y += (ran - 0.5f) * 0.1f /*+ diffuse.x*/;
//
//		float4 gate = tex2.Sample(samLinear, gateUV);
//
//
//		return lerp(base, gate, step(1, distortion));
//
//	}
//	else
//	{
//		//static float count = 0.0f;
//		//count += 0.1f;
//		float2 center2 = float2(0.7f, 0.2f);
//		float dist2 = distance(center2, input.Tex);
//		float r2 = diffuse.y - 1.3f;
//		float distortion2 = 1 - smoothstep(r2 - 0.07f, r2, dist2);
//		float2 uv2 = input.Tex + (center2 - input.Tex) * distortion2;
//
//
//		float4 base2 = tex2.Sample(samLinear, uv2);
//
//		float2 gateUV2 = input.Tex;
//		float ran2 = perlinNoise(input.Tex * 20);
//		gateUV2.y += (ran2 - 0.5f) * 0.1f + diffuse.x;
//
//		float4 gate2 = tex.Sample(samLinear, gateUV2);
//
//
//		return lerp(base2, gate2, step(1, distortion2));
//
//	}
//}

//float4 main(PS_INPUT input) : SV_TARGET
//{
	//float4 base = tex.Sample(samLinear, input.Tex);
	//float4 base = float4(0,0,0,diffuse.x);

	//if (input.Tex.x < 0)
	//{
	//	base = float4 (0, 0, 0, 1);
	//}
	//if (input.Tex.x > 1)
	//{
	//	base = float4 (0, 0, 0, 1);
	//}
	//if (input.Tex.y < 0)
	//{
	//	base = float4 (0, 0, 0, 1);
	//}
	//if (input.Tex.y > 1)
	//{
	//	base = float4 (0, 0, 0, 1);
	//}

	//return base;
//}
//{
//	if (diffuse.y < 1.3f) 
//	{
//		float2 center = float2(0.7f, 0.2f);
//		float dist = distance(center, input.Tex);
//		float r = diffuse.y;
//		float distortion = 1 - smoothstep(r - 0.07f, r, dist);
//		float2 uv = input.Tex /*+ (center - input.Tex) * distortion*/;
//
//
//		float4 base = tex.Sample(samLinear, uv);
//
//		float2 gateUV = input.Tex;
//		float ran = perlinNoise(input.Tex * 20);
//		//gateUV.y += (ran - 0.5f) * 0.1f /*+ diffuse.x*/;
//
//		float4 gate = tex2.Sample(samLinear, gateUV);
//
//
//		return lerp(base, gate, step(1, distortion));
//
//	}
//	else
//	{
//		//static float count = 0.0f;
//		//count += 0.1f;
//		float2 center2 = float2(0.7f, 0.2f);
//		float dist2 = distance(center2, input.Tex);
//		float r2 = diffuse.y - 1.3f;
//		float distortion2 = 1 - smoothstep(r2 - 0.07f, r2, dist2);
//		float2 uv2 = input.Tex + (center2 - input.Tex) * distortion2;
//
//
//		float4 base2 = tex2.Sample(samLinear, uv2);
//
//		float2 gateUV2 = input.Tex;
//		float ran2 = perlinNoise(input.Tex * 20);
//		gateUV2.y += (ran2 - 0.5f) * 0.1f + diffuse.x;
//
//		float4 gate2 = tex.Sample(samLinear, gateUV2);
//
//
//		return lerp(base2, gate2, step(1, distortion2));
//
//	}
//}

float distortion(float2 center, float r, float d_r, float2 tx)
{
	float dist = distance(center, tx);

	float distortion = 1 - smoothstep(r - d_r, r, dist);

	return distortion;
}

float4 DistortionColor(float2 center, float r, float d_r, float2 tx)
{
	float d = distortion(center, r, d_r, tx);

	float2 uv = lerp(tx, center, d);


	float4 base = tex.Sample(samLinear, uv);

	float4 L1 = lerp(base, float4(0, 0, 0, 0), step(1, d));

	return L1;

}

float grayscale(float4 col)
{
	float gray = col.r * 0.2f + col.g *0.7f + col.b * 0.1f;

	return gray;
}

float4 portal(float2 center, float r, float d_r, float2 tx)
{
	return DistortionColor(center, r, d_r, tx);
}

//float4 main(PS_INPUT input) : SV_TARGET
//{
//			float2 center = float2(0.7f, 0.2f);
//			float r = 0.3f;
//			float d_r = 0.07f;
//
//
//			float4 base = DistortionColor(center, r, d_r, input.Tex);
//
//			base.rgb = grayscale(base);
//
//			return base;
//
//}

float4 blur(float2 n, float2 tx)
{
	//float2 n = float2 (0.005f, 0.005f);
	float4 base = tex.Sample(samLinear, tx + float2( n.x,  n.y))
				+ tex.Sample(samLinear, tx + float2( n.x,    0))
				+ tex.Sample(samLinear, tx + float2( n.x, -n.y))
				+ tex.Sample(samLinear, tx + float2(   0,  n.y))
				+ tex.Sample(samLinear, tx + float2(   0,    0))
				+ tex.Sample(samLinear, tx + float2(   0, -n.y))
				+ tex.Sample(samLinear, tx + float2(-n.x,  n.y))
				+ tex.Sample(samLinear, tx + float2(-n.x,    0))
				+ tex.Sample(samLinear, tx + float2(-n.x, -n.y));

	base /= 9;
	return base;
}

float4 dissolve(float2 tx, float holeSize)
{

	float r = perlinNoise(tx * holeSize);

	float4 base = tex.Sample(samLinear, tx);

	if (r < -diffuse.x)
	{
		base.a = 0;
	}
	return base;

}


float2 mod(float2 a, float2 b)
{
	return a - floor(a / b) * b;
}

float4 main(PS_INPUT input) : SV_TARGET
{

	float2 uv = input.Tex;
	uv = uv - 0.5;

	float vignet = length(uv);
	uv /= 1 - vignet * 0.2f;

	float2 texUV = uv + 0.5f;
	//if (texUV.x < 0 || texUV.x > 1 || texUV.y < 0 || texUV.y > 1)
	if (max(abs(uv.y) - 0.5f, abs(uv.x) - 0.5f) > 0)
	{
		return tex2.Sample(samLinear, input.Tex);
	}
	
	float3 color;

	texUV.x += sin(texUV.y + 100) * 0.002f;
	texUV += 0.001f;
	texUV.x += (random(floor(texUV.y * 100) + diffuse.x) - 0.5f) * 0.01f;

	texUV = mod(texUV, 1);

	color.r = tex.Sample(samLinear, texUV).r;
	color.g = tex.Sample(samLinear, texUV + float2(0.002f, 0)).g;
	color.b = tex.Sample(samLinear, texUV + float2(0.004f, 0)).b;

	if (random((random(floor(texUV.y * 500) + diffuse.x) - 0.5f) + diffuse.x) < 0.001f)
	{
		color.r = random(uv + float2(123 + diffuse.x, 0));
		color.g = random(uv + float2(123 + diffuse.x, 1));
		color.b = random(uv + float2(123 + diffuse.x, 2));
	}
	color *= 1 - vignet * 1.3f;
	float4 base = tex.Sample(samLinear, texUV);

	base = float4(color, base.a);
	return base;
}


//float4 main(PS_INPUT input) : SV_TARGET
//{
//
//	return dissolve(input.Tex, diffuse.x);
//}