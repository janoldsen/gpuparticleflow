
#include "Header.h"

static const int perm[512] = {
	151, 160, 137, 91, 90, 15, 131, 13, 201, 95, 96, 53, 194, 233, 7, 225, 140, 36, 103, 30, 69, 142,
	8, 99, 37, 240, 21, 10, 23, 190, 6, 148, 247, 120, 234, 75, 0, 26, 197, 62, 94, 252, 219, 203, 117,
	35, 11, 32, 57, 177, 33, 88, 237, 149, 56, 87, 174, 20, 125, 136, 171, 168, 68, 175, 74, 165, 71,
	134, 139, 48, 27, 166, 77, 146, 158, 231, 83, 111, 229, 122, 60, 211, 133, 230, 220, 105, 92, 41,
	55, 46, 245, 40, 244, 102, 143, 54, 65, 25, 63, 161, 1, 216, 80, 73, 209, 76, 132, 187, 208, 89,
	18, 169, 200, 196, 135, 130, 116, 188, 159, 86, 164, 100, 109, 198, 173, 186, 3, 64, 52, 217, 226,
	250, 124, 123, 5, 202, 38, 147, 118, 126, 255, 82, 85, 212, 207, 206, 59, 227, 47, 16, 58, 17, 182,
	189, 28, 42, 223, 183, 170, 213, 119, 248, 152, 2, 44, 154, 163, 70, 221, 153, 101, 155, 167, 43,
	172, 9, 129, 22, 39, 253, 19, 98, 108, 110, 79, 113, 224, 232, 178, 185, 112, 104, 218, 246, 97,
	228, 251, 34, 242, 193, 238, 210, 144, 12, 191, 179, 162, 241, 81, 51, 145, 235, 249, 14, 239,
	107, 49, 192, 214, 31, 181, 199, 106, 157, 184, 84, 204, 176, 115, 121, 50, 45, 127, 4, 150, 254,
	138, 236, 205, 93, 222, 114, 67, 29, 24, 72, 243, 141, 128, 195, 78, 66, 215, 61, 156, 180,

	151, 160, 137, 91, 90, 15, 131, 13, 201, 95, 96, 53, 194, 233, 7, 225, 140, 36, 103, 30, 69, 142,
	8, 99, 37, 240, 21, 10, 23, 190, 6, 148, 247, 120, 234, 75, 0, 26, 197, 62, 94, 252, 219, 203, 117,
	35, 11, 32, 57, 177, 33, 88, 237, 149, 56, 87, 174, 20, 125, 136, 171, 168, 68, 175, 74, 165, 71,
	134, 139, 48, 27, 166, 77, 146, 158, 231, 83, 111, 229, 122, 60, 211, 133, 230, 220, 105, 92, 41,
	55, 46, 245, 40, 244, 102, 143, 54, 65, 25, 63, 161, 1, 216, 80, 73, 209, 76, 132, 187, 208, 89,
	18, 169, 200, 196, 135, 130, 116, 188, 159, 86, 164, 100, 109, 198, 173, 186, 3, 64, 52, 217, 226,
	250, 124, 123, 5, 202, 38, 147, 118, 126, 255, 82, 85, 212, 207, 206, 59, 227, 47, 16, 58, 17, 182,
	189, 28, 42, 223, 183, 170, 213, 119, 248, 152, 2, 44, 154, 163, 70, 221, 153, 101, 155, 167, 43,
	172, 9, 129, 22, 39, 253, 19, 98, 108, 110, 79, 113, 224, 232, 178, 185, 112, 104, 218, 246, 97,
	228, 251, 34, 242, 193, 238, 210, 144, 12, 191, 179, 162, 241, 81, 51, 145, 235, 249, 14, 239,
	107, 49, 192, 214, 31, 181, 199, 106, 157, 184, 84, 204, 176, 115, 121, 50, 45, 127, 4, 150, 254,
	138, 236, 205, 93, 222, 114, 67, 29, 24, 72, 243, 141, 128, 195, 78, 66, 215, 61, 156, 180
};





float getRandom(int x, int y, int z, int t, int seed)
{
	x = x & 255;
	y = y & 255;
	z = z & 255;
	t = t & 255;

	seed = seed & 255;
	return (perm[x + perm[y + perm[z + perm[t + perm[seed]]]]] % 12 / 12.0f * 2.0f) - 1.0f;
}


float noise(float3 pos, float t, int seed)
{

	int floorX = floor(pos.x);
	int floorY = floor(pos.y);
	int floorZ = floor(pos.z);
	int floorT = floor(t);

	float fractX = pos.x - floorX;
	float fractY = pos.y - floorY;
	float fractZ = pos.z - floorZ;
	float fractT = t - floorT;



	float s11 = getRandom(floorX, floorY, floorZ, floorT, seed);
	float t11 = getRandom(floorX + 1, floorY, floorZ, floorT, seed);
	float u11 = getRandom(floorX, floorY + 1, floorZ, floorT, seed);
	float v11 = getRandom(floorX + 1, floorY + 1, floorZ, floorT, seed);

	float s12 = getRandom(floorX, floorY, floorZ + 1, floorT, seed);
	float t12 = getRandom(floorX + 1, floorY, floorZ + 1, floorT, seed);
	float u12 = getRandom(floorX, floorY + 1, floorZ + 1, floorT, seed);
	float v12 = getRandom(floorX + 1, floorY + 1, floorZ + 1, floorT, seed);

	float i111 = s11 * (1.0f - fractX) + t11 * fractX;
	float i112 = u11 * (1.0f - fractX) + v11 * fractX;
	float i113 = i111 * (1.0f - fractY) + i112 * fractY;

	float i121 = s12 * (1.0f - fractX) + t12 * fractX;
	float i122 = u12 * (1.0f - fractX) + v12 * fractX;
	float i123 = i121 * (1.0f - fractY) + i122 * fractY;

	float i1 = i113 * (1.0f - fractZ) + i123 * fractZ;


	float s21 = getRandom(floorX, floorY, floorZ, floorT + 1, seed);
	float t21 = getRandom(floorX + 1, floorY, floorZ, floorT + 1, seed);
	float u21 = getRandom(floorX, floorY + 1, floorZ, floorT + 1, seed);
	float v21 = getRandom(floorX + 1, floorY + 1, floorZ, floorT + 1, seed);

	float s22 = getRandom(floorX, floorY, floorZ + 1, floorT + 1, seed);
	float t22 = getRandom(floorX + 1, floorY, floorZ + 1, floorT + 1, seed);
	float u22 = getRandom(floorX, floorY + 1, floorZ + 1, floorT + 1, seed);
	float v22 = getRandom(floorX + 1, floorY + 1, floorZ + 1, floorT + 1, seed);

	float i211 = s21 * (1.0f - fractX) + t21 * fractX;
	float i212 = u21 * (1.0f - fractX) + v21 * fractX;
	float i213 = i211 * (1.0f - fractY) + i212 * fractY;

	float i221 = s22 * (1.0f - fractX) + t22 * fractX;
	float i222 = u22 * (1.0f - fractX) + v22 * fractX;
	float i223 = i221 * (1.0f - fractY) + i222 * fractY;

	float i2 = i213 * (1.0f - fractZ) + i223 * fractZ;

	return i1 * (1.0 - fractT) + i2 * fractT;
}



float3 getVel(float3 pos, float t, int seedX, int seedY, int seedZ)
{

	float3 v = float3(0,0,0);

	int octaves = 16;
	for (int i = 0; i < octaves; i++)
	{
		float f = 1.0f / pow(2.0f, 1);

		v.x += noise(float3(pos.x, pos.y, pos.z) * 0.01f * f, t * 0.01f * f, seedX);

		v.y += noise(float3(pos.x, pos.y, pos.z) * 0.01f * f, t * 0.01f * f, seedY);

		v.z += noise(float3(pos.x, pos.y, pos.z) * 0.01f * f, t * 0.01f * f, seedZ);

	}
	v /= (float)octaves;

	float turbPower = 2;


	return v;

}




cbuffer ConstantData
{
	float seedX;
	float seedY;
	float seedZ;

	float xScale;
	float yScale;
	float zScale;
	float timeScale;

	float maxXVelocity;
	float minXVelocity;
	float maxYVelocity;
	float minYVelocity;
	float maxZVelocity;
	float minZVelocity;
	float lifeTime;

};

cbuffer FrameData
{
	float t;
	float dt;
	float3 eye;
	//matrix view;
};





RWStructuredBuffer<ParticleData> data;
AppendStructuredBuffer<uint> deadList;
RWStructuredBuffer<AliveData> aliveList;


[numthreads(1024, 1, 1)]
void CSMain(uint3 id : SV_DispatchThreadID)
{
	if (data[id.x].lifeTime > 0.0f)
	{
		float3 p = data[id.x].position;


		p.x *= xScale;
		p.y *= yScale;
		p.z *= zScale;

		float3 v = getVel(p, t * timeScale, seedX, seedY, seedZ);

			clamp(v, -1.0, 1.0);


		v.x = (v.x + 1) * (maxXVelocity - minXVelocity) / 2.0f + minXVelocity;
		v.y = (v.y + 1) * (maxYVelocity - minYVelocity) / 2.0f + minYVelocity;
		v.z = (v.z + 1) * (maxZVelocity - minZVelocity) / 2.0f + minZVelocity;


		data[id.x].position += v * dt;
		data[id.x].lifeTime -= dt;
		data[id.x].velocity = v;


		

		if (data[id.x].lifeTime < 0.0f)
		{


			deadList.Append(id.x);
		}
		else
		{
			uint idx = aliveList.IncrementCounter();

			AliveData aliveData;
			aliveData.id = id.x;
			//aliveData.depth = mul(float4(data[id.x].position, 1), view).z;
			aliveData.depth = length(data[id.x].position - eye);
			
			
			aliveList[idx] = aliveData;
		}

	}
}