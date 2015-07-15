#include "Header.h"


RWStructuredBuffer<ParticleData> data;
ConsumeStructuredBuffer<uint> deadList;

Texture2D noiseTexture;

cbuffer SpawnData
{
	float lifeTime;
	float spawnTime;
	int maxCount;
	float spawnRadiusX;
	float spawnRadiusY;
	float spawnRadiusZ;
	float scale;
}

cbuffer FrameBuffer
{
	float t;
	uint toSpawn;
}

cbuffer DeadListCount
{
	uint numDeadParticles;
}

SamplerState TexSampler
{
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = Wrap;
	AddressV = Wrap;
};


[numthreads(1024, 1, 1)]
void CSMain(uint3 id : SV_DispatchThreadID)
{
	if (id.x < numDeadParticles && id.x < toSpawn)
	{

		float2 uv = float2(id.x/(float)toSpawn, (t%10) / 10.0f);
		float3 noise = noiseTexture.SampleLevel(TexSampler, uv, 0).xyz;
		

		ParticleData particle;

		particle.position[0] = noise.x * spawnRadiusX;

		float rY = sqrt(pow(spawnRadiusY, 2) - pow(particle.position[0], 2));
		particle.position[1] = noise.y * rY;

		float rZ = sqrt(pow(spawnRadiusZ, 2) - (pow(particle.position[0], 2) + pow(particle.position[1], 2)));
		particle.position[2] = noise.z * rZ;

		particle.lifeTime = lifeTime;
		particle.velocity = float3(0, 0, 0);

		uint idx = deadList.Consume();

		data[idx] = particle;

	}
}