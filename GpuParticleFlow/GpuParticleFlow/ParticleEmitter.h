#ifndef PARTICLE_EMITTER_H
#define PARTICLE_EMITTER_H

#include <stack>
#include <vector>
#include "graphics.h"

#include "Matrix4x4.h"

class Camera;

struct ConstantData
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
};

struct SimulateFrameData
{
	float t;
	float dt;
	Vector3f eye;
	//Matrix4x4f view;
};

struct EmitterFrameData
{
	float t;
	UINT toSpawn;
};

struct RenderData
{
	Matrix4x4f World;
	Matrix4x4f View;
	Matrix4x4f ViewInverse;
	Matrix4x4f Projection;
	float maxLifeTime;
	float scale;
	float maxV;
};

struct SpawnData
{
	float lifeTime;
	float spawnTime;
	int maxCount;
	float spawnRadiusX;
	float spawnRadiusY;
	float spawnRadiusZ;
	float scale;
};



struct ParticleData
{
	float position[3];
	float velocity[3];
	float lifeTime;
};

struct SortConstants
{
	int numElements;
	int compareDistance;
	int sortConstantsX;
	int sortConstantsY;
};
	
struct AliveData
{
	int id;
	float depth;
};

class ParticleEmitter
{
public:

	ParticleEmitter(const ConstantData& constData, const SpawnData& spawnData, ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	~ParticleEmitter();


	void addParticle1(int num, ID3D11DeviceContext* pDeviceContext);
	void addParticle(int num, ID3D11DeviceContext* pDeviceContext);

	void sort(ID3D11DeviceContext* pDeviceContext);

	void update(float  dt, ID3D11DeviceContext* pDeviceContext, Camera* pCamera);	
	void draw(ID3D11DeviceContext* pDeviceContext, Camera* pCamera);

	void fillDeadList(ID3D11DeviceContext* pDeviceContext);
	
	int getCount();


	
private:

	unsigned int m_count;
	
	float m_timer;

	float m_time;

	float* m_lifeTimes;
	std::stack<int> m_freeIDs;
	//std::vector<int> m_freeIDs;

	std::stack<int> m_toAdd;
	
	SpawnData m_spawnData;
	ConstantData m_constantData;
		
	
	ID3D11ComputeShader* m_pCSSimulate;
	ID3D11ComputeShader* m_pCSEmit;
	ID3D11ComputeShader* m_pCSFillDead;
	
	ID3D11ComputeShader* m_pCSSortInitial;
	ID3D11ComputeShader* m_pCSSortOuter;
	ID3D11ComputeShader* m_pCSSortInner;

	ID3D11VertexShader* m_pVertexShader;
	ID3D11GeometryShader* m_pGeometryShader;
	ID3D11PixelShader* m_pPixelShader;

	
	ID3D11Buffer* m_pCountBuffer;

	ID3D11Buffer* m_pParticleBuffer;
	ID3D11Buffer* m_pDeadBuffer;
	ID3D11Buffer* m_pAliveBuffer;
	
	//
	ID3D11Buffer* m_pTEMPBuffer;
	//

	ID3D11Buffer* m_pSortConstantsBuffer;

	ID3D11Buffer* m_pConstantDataBuffer;
	ID3D11Buffer* m_pSimulateFrameDataBuffer;

	ID3D11Buffer* m_pSpawnDataBuffer;
	ID3D11Buffer* m_pEmitterFrameDataBuffer;
	ID3D11Buffer* m_pNumDeadBuffer;

	ID3D11Texture2D* m_pNoiseTexture;
	ID3D11ShaderResourceView* m_pNoiseSRV;

	ID3D11UnorderedAccessView* m_pParticleUAV;
	ID3D11UnorderedAccessView* m_pDeadUAV;
	ID3D11UnorderedAccessView* m_pAliveUAV;

	ID3D11ShaderResourceView* m_pParticleSRV;
	ID3D11ShaderResourceView* m_pAliveSRV;

	ID3D11Buffer* m_pVertexBuffer;
	ID3D11Buffer* m_pConstantRenderBuffer;

	ID3D11Texture2D* m_pSpriteTexture;
	ID3D11ShaderResourceView* m_pSpriteSRV;

};


#endif