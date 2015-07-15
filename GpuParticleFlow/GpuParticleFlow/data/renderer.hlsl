

#include "Header.h"


StructuredBuffer<ParticleData> particles;
StructuredBuffer<AliveData> alive;

texture2D sprite;


cbuffer buffer
{
	matrix World;
	matrix View;
	matrix ViewInverse;
	matrix Projection;
	float maxLifeTime;
	float scale;
	float maxV;
};


//
//struct VS_Input
//{
//	float3 position : SV_Position;
//};

struct GS_Input
{
	ParticleData particle : TEXCOORD0;
};

struct PS_Input
{
	float4 position : SV_Position;
	float2 uv : TEXCOORD0;
	float lifeTime : TEXCOORD1;
	float4 color : TEXCOORD2;
};




GS_Input VSMain(/*VS_Input input,*/ uint id : SV_VertexId)
{
	GS_Input output = (GS_Input)0;

	uint idx = alive[id].id;
	

	output.particle = particles[idx];

	return output;
}


[maxvertexcount(4)]
void GSMain(point GS_Input input[1], inout TriangleStream<PS_Input> outputStream)
{
	PS_Input output[4];

	
	float3 up = normalize(ViewInverse[1].xyz);
	float3 right = normalize(ViewInverse[0].xyz);

	float3 pos = input[0].particle.position;

	
	output[0].position = float4(pos - (right + up) * scale,1);
	output[0].uv = float2(0, 1);
	output[1].position = float4(pos - (right - up) * scale, 1);
	output[1].uv = float2(0, 0);
	output[2].position = float4(pos + (right - up) * scale, 1);
	output[2].uv = float2(1, 1);
	output[3].position = float4(pos + (right + up) * scale, 1);
	output[3].uv = float2(1, 0); 
	



	for (int i = 0; i < 4; i++)
	{
		output[i].position = mul(mul(mul(output[i].position, World), View), Projection);
		output[i].lifeTime = input[0].particle.lifeTime / maxLifeTime;
		output[i].color = lerp(float4(1, 0, 0, 1), float4(0, 0, 1, 1), 2 *  length(input[0].particle.velocity) / maxV);

		outputStream.Append(output[i]);
	}
	outputStream.RestartStrip();
}

SamplerState TexSampler
{
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = Wrap;
	AddressV = Wrap;
};



float4 PSMain(PS_Input input) : SV_TARGET
{
	float4 color = sprite.Sample(TexSampler, input.uv).rgba;
	
	//clip(color.a - 0.4);

	color.a *= -4 * pow(input.lifeTime - 0.5, 2) + 1;

	color *= input.color;

	return color;
}
