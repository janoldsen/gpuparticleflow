
#include "ParticleEmitter.h"

#include <iostream>
#include "Camera.h"
#include "D3DX11.h"
#include <algorithm>


#include <assert.h>


ParticleEmitter::ParticleEmitter(const ConstantData& constData, const  SpawnData& spawnData, ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: m_constantData(constData),
	m_spawnData(spawnData)

{


	m_count = 0;
	
	m_lifeTimes = new float[m_spawnData.maxCount];

	for (int i = 0; i < m_spawnData.maxCount; ++i)
	{
		m_lifeTimes[i] = 0;
		m_freeIDs.push(i);
	}
	//std::sort(m_freeIDs.begin(), m_freeIDs.end());


	// initialize Shaders
	{
		printf("compiling shaders...\n");
		ID3D10Blob* blob = nullptr;
		CompileShader(L"data\\Simulator.hlsl", "CSMain", "cs_5_0", pDevice, &blob);
		pDevice->CreateComputeShader(blob->GetBufferPointer(), blob->GetBufferSize(), 0, &m_pCSSimulate);

		blob = nullptr;
		CompileShader(L"data\\Emitter.hlsl", "CSMain", "cs_5_0", pDevice, &blob);
		pDevice->CreateComputeShader(blob->GetBufferPointer(), blob->GetBufferSize(), 0, &m_pCSEmit);
		
		blob = nullptr;
		CompileShader(L"data\\fillDeadList.hlsl", "CSMain", "cs_5_0", pDevice, &blob);
		pDevice->CreateComputeShader(blob->GetBufferPointer(), blob->GetBufferSize(), 0, &m_pCSFillDead);


		blob = nullptr;
		CompileShader(L"data\\sorter.hlsl", "CSSortInitial", "cs_5_0", pDevice, &blob);
		pDevice->CreateComputeShader(blob->GetBufferPointer(), blob->GetBufferSize(), 0, &m_pCSSortInitial);

		blob = nullptr;
		CompileShader(L"data\\sorter.hlsl", "CSSortOuter", "cs_5_0", pDevice, &blob);
		pDevice->CreateComputeShader(blob->GetBufferPointer(), blob->GetBufferSize(), 0, &m_pCSSortOuter);
		
		blob = nullptr;
		CompileShader(L"data\\sorter.hlsl", "CSSortInner", "cs_5_0", pDevice, &blob);
		pDevice->CreateComputeShader(blob->GetBufferPointer(), blob->GetBufferSize(), 0, &m_pCSSortInner);

		blob = nullptr;
		CompileShader(L"data\\renderer.hlsl", "VSMain", "vs_5_0", pDevice, &blob);
		pDevice->CreateVertexShader(blob->GetBufferPointer(), blob->GetBufferSize(), 0, &m_pVertexShader);

		D3D11_INPUT_ELEMENT_DESC elem[] =
		{
			{ "SV_POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};

		ID3D11InputLayout* pInputLayout;
		pDevice->CreateInputLayout(elem, 1, blob->GetBufferPointer(), blob->GetBufferSize(), &pInputLayout);
		pDeviceContext->IASetInputLayout(pInputLayout);
		pInputLayout->Release();


		blob = nullptr;
		CompileShader(L"data\\renderer.hlsl", "GSMain", "gs_5_0", pDevice, &blob);
		pDevice->CreateGeometryShader(blob->GetBufferPointer(), blob->GetBufferSize(), 0, &m_pGeometryShader);

		blob = nullptr;
		CompileShader(L"data\\renderer.hlsl", "PSMain", "ps_5_0", pDevice, &blob);
		pDevice->CreatePixelShader(blob->GetBufferPointer(), blob->GetBufferSize(), 0, &m_pPixelShader);



	}


	// intitialize buffers;
	{
		// buffers
		{
			D3D11_BUFFER_DESC desc =
			{
				m_spawnData.maxCount * sizeof(ParticleData),
				D3D11_USAGE_DEFAULT, 
				D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS,
				0, 
				D3D11_RESOURCE_MISC_BUFFER_STRUCTURED,
				sizeof(ParticleData)
			};

			pDevice->CreateBuffer(&desc, 0, &m_pParticleBuffer);

			desc.ByteWidth = sizeof(UINT) * m_spawnData.maxCount;
			desc.StructureByteStride = sizeof(UINT);
			
			pDevice->CreateBuffer(&desc, 0, &m_pDeadBuffer);

			desc.ByteWidth = sizeof(AliveData) * m_spawnData.maxCount;
			desc.StructureByteStride = sizeof(AliveData);

			pDevice->CreateBuffer(&desc, 0, &m_pAliveBuffer);

			desc.ByteWidth = sizeof(m_count);
			desc.Usage = D3D11_USAGE_STAGING;
			desc.MiscFlags = 0;
			desc.BindFlags = 0;
			desc.CPUAccessFlags = D3D10_CPU_ACCESS_READ;
			
			pDevice->CreateBuffer(&desc, 0, &m_pCountBuffer);

			desc.ByteWidth = sizeof(AliveData) * m_spawnData.maxCount;
			pDevice->CreateBuffer(&desc, 0, &m_pTEMPBuffer);

		}

		// unordered view
		{
			D3D11_UNORDERED_ACCESS_VIEW_DESC desc;
	
			desc.Format = DXGI_FORMAT_UNKNOWN;
			desc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
			desc.Buffer.FirstElement = 0;
			desc.Buffer.NumElements = m_spawnData.maxCount;
			desc.Buffer.Flags = 0;

			pDevice->CreateUnorderedAccessView(m_pParticleBuffer, &desc, &m_pParticleUAV);

			desc.Buffer.Flags = D3D11_BUFFER_UAV_FLAG_APPEND;
			pDevice->CreateUnorderedAccessView(m_pDeadBuffer, &desc, &m_pDeadUAV);
			desc.Buffer.Flags = D3D11_BUFFER_UAV_FLAG_COUNTER;
			pDevice->CreateUnorderedAccessView(m_pAliveBuffer, &desc, &m_pAliveUAV);
			
		}

		// shader view
		{
			D3D11_SHADER_RESOURCE_VIEW_DESC desc = 
			{
				DXGI_FORMAT_UNKNOWN,
				D3D11_SRV_DIMENSION_BUFFEREX,
				{ 0, m_spawnData.maxCount }
			};

			pDevice->CreateShaderResourceView(m_pParticleBuffer, &desc, &m_pParticleSRV);
			pDevice->CreateShaderResourceView(m_pAliveBuffer, &desc, &m_pAliveSRV);
		}

		
		// constant buffers
		{
			D3D11_BUFFER_DESC desc = 
			{
				ceil(sizeof(ConstantData)/16.0f)*16,
				D3D11_USAGE_DYNAMIC,
				D3D11_BIND_CONSTANT_BUFFER,
				D3D11_CPU_ACCESS_WRITE,
				0,
				0
			};

			D3D11_SUBRESOURCE_DATA initData;
			initData.pSysMem = &m_constantData;
			initData.SysMemSlicePitch = 0;
			initData.SysMemPitch = 0;

			pDevice->CreateBuffer(&desc, &initData, &m_pConstantDataBuffer);


			desc.ByteWidth = ceil(sizeof(SimulateFrameData) / 16.0f) * 16;

			pDevice->CreateBuffer(&desc, 0, &m_pSimulateFrameDataBuffer);

			desc.ByteWidth = ceil(sizeof(RenderData) / 16.0f) * 16;

			pDevice->CreateBuffer(&desc, 0, &m_pConstantRenderBuffer);

			initData.pSysMem = &m_spawnData;
			desc.ByteWidth = ceil(sizeof(SpawnData) / 16.0f) *16.0f;
			pDevice->CreateBuffer(&desc, &initData, &m_pSpawnDataBuffer);

			desc.ByteWidth = ceil(sizeof(EmitterFrameData) / 16.0f) * 16;
			pDevice->CreateBuffer(&desc, 0, &m_pEmitterFrameDataBuffer);

			desc.ByteWidth = 16;
			pDevice->CreateBuffer(&desc, 0, &m_pNumDeadBuffer);

			desc.ByteWidth = sizeof(SortConstants);
			pDevice->CreateBuffer(&desc, 0, &m_pSortConstantsBuffer);
			
		}

		// Vertex buffer
		{
			D3D11_BUFFER_DESC desc =
			{
				sizeof(float) * 3 * m_spawnData.maxCount,
				D3D11_USAGE_DEFAULT,
				D3D11_BIND_VERTEX_BUFFER,
				0,
				0,
				0
			};

			pDevice->CreateBuffer(&desc, 0, &m_pVertexBuffer);

			
		}

		// texture
		{
			ID3D11Resource* res;
			D3DX11CreateTextureFromFile(pDevice, L"data\\licht.png", 0, NULL, &res, NULL);
			m_pSpriteTexture = (ID3D11Texture2D*)res;

			D3D11_TEXTURE2D_DESC textDesc;
			m_pSpriteTexture->GetDesc(&textDesc);

			D3D11_SHADER_RESOURCE_VIEW_DESC desc = 
			{
				textDesc.Format,
				D3D11_SRV_DIMENSION_TEXTURE2D,
				{ 0, 1 }
			};

			pDevice->CreateShaderResourceView(m_pSpriteTexture, &desc, &m_pSpriteSRV);


			GetNoiseMap(1024, 1024, pDevice, &m_pNoiseTexture);

			desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
			pDevice->CreateShaderResourceView(m_pNoiseTexture, &desc, &m_pNoiseSRV);

		}
	
	}

	fillDeadList(pDeviceContext);
}

ParticleEmitter::~ParticleEmitter()
{
	delete[] m_lifeTimes;


	
	std::cout << "Releasing Shaders" << std::endl;

	m_pCSSimulate->Release();
	m_pCSEmit->Release();
	m_pCSFillDead->Release();
	m_pVertexShader->Release();
	m_pGeometryShader->Release();
	m_pPixelShader->Release();

	m_pCSSortInitial->Release();
	m_pCSSortOuter->Release();
	m_pCSSortInner->Release();

	
	std::cout << "Releasing Buffers" << std::endl;

	m_pCountBuffer->Release();

	m_pParticleBuffer->Release();
	m_pDeadBuffer->Release();
	m_pAliveBuffer->Release();

	m_pConstantDataBuffer->Release();
	m_pSimulateFrameDataBuffer->Release();

	m_pSpawnDataBuffer->Release();
	m_pEmitterFrameDataBuffer->Release();
	m_pNumDeadBuffer->Release();
	m_pConstantRenderBuffer->Release();
	m_pSortConstantsBuffer->Release();


	std::cout << "Releasing UAVs" << std::endl;

	m_pParticleUAV->Release();
	m_pDeadUAV->Release();
	m_pAliveUAV->Release();

	std::cout << "Releasing Textures" << std::endl;

	m_pNoiseTexture->Release();
	m_pSpriteTexture->Release();

	std::cout << "Releasing SRVs" << std::endl;

	m_pNoiseSRV->Release();
	m_pParticleSRV->Release();
	m_pAliveSRV->Release();
	m_pSpriteSRV->Release();

	m_pVertexBuffer->Release();

	std::cout << "Done" << std::endl;
}


void ParticleEmitter::fillDeadList(ID3D11DeviceContext* pDeviceContext)
{
	pDeviceContext->CSSetShader(m_pCSFillDead, 0, 0);

	UINT initCount[1] = { 0 };
	pDeviceContext->CSSetUnorderedAccessViews(0, 1, &m_pDeadUAV, initCount);

	pDeviceContext->Dispatch(ceil(m_spawnData.maxCount / 1024.0f), 1, 1);
}


void ParticleEmitter::addParticle(int num, ID3D11DeviceContext* pDeviceContext)
{
	if (m_count + num > m_spawnData.maxCount)
		num = m_spawnData.maxCount - m_count;

	// update num dead
	{
		pDeviceContext->CopyStructureCount(m_pNumDeadBuffer, 0, m_pDeadUAV);

	}

	// update Frame Data
	{
		EmitterFrameData frameDataSrc;
		frameDataSrc.t = m_time;
		frameDataSrc.toSpawn = num;


		D3D11_MAPPED_SUBRESOURCE frameDataDst;

		pDeviceContext->Map(m_pEmitterFrameDataBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &frameDataDst);
		*((EmitterFrameData*)frameDataDst.pData) = frameDataSrc;
		pDeviceContext->Unmap(m_pEmitterFrameDataBuffer, 0);
	}



	pDeviceContext->CSSetShader(m_pCSEmit, 0, 0);

	// set UAVS
	{
		ID3D11UnorderedAccessView* UAVs[2] = { m_pParticleUAV, m_pDeadUAV };
		UINT initCounts[2] = { (UINT)-1, (UINT)-1 };

		pDeviceContext->CSSetUnorderedAccessViews(0, 2, UAVs, initCounts);
	}

	// set cBuffers
	{
		ID3D11Buffer* cBuffers[3] = { m_pSpawnDataBuffer, m_pEmitterFrameDataBuffer, m_pNumDeadBuffer };
		pDeviceContext->CSSetConstantBuffers(0, 3, cBuffers);
	}

	// set SRVs
	pDeviceContext->CSSetShaderResources(0, 1, &m_pNoiseSRV);

	pDeviceContext->Dispatch(ceil(num / 1024.0f),1,1);


}

void ParticleEmitter::sort(ID3D11DeviceContext* pDeviceContext)
{

	SortConstants sortConstantsSrc;
	sortConstantsSrc.numElements = m_count;


	//set sortconstants
	D3D11_MAPPED_SUBRESOURCE sortConstantsDst;
	pDeviceContext->Map(m_pSortConstantsBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &sortConstantsDst);
	*((SortConstants*)sortConstantsDst.pData) = sortConstantsSrc;
	pDeviceContext->Unmap(m_pSortConstantsBuffer, 0);


	UINT initialCount[1] = { UINT(-1) };

	pDeviceContext->CSSetUnorderedAccessViews(0, 1, &m_pAliveUAV, initialCount);
	pDeviceContext->CSSetConstantBuffers(0, 1, &m_pSortConstantsBuffer);

	// sort from 2 to 2048

	pDeviceContext->CSSetShader(m_pCSSortInitial, 0, 0);

	int numThreadGroups = m_count > 2048 ? ((m_count - 1) >> 11) + 1 : 1;
	pDeviceContext->Dispatch(numThreadGroups, 1, 1);


	// sort rest

	for (int subArraySize = 4096; subArraySize < 2*m_count; subArraySize = subArraySize <<1)
	{
		pDeviceContext->CSSetShader(m_pCSSortOuter, 0, 0);

		int pow2 = subArraySize / 2;
		while (pow2 < m_count)
		{
			pow2 *= 2;
		}
		numThreadGroups = pow2 >> 11;

		int compareDistance = subArraySize >> 1;
		// sort first
		{
			// set constants
			{
				sortConstantsSrc.compareDistance = compareDistance;
				sortConstantsSrc.sortConstantsX = (2 * compareDistance - 1);
				sortConstantsSrc.sortConstantsY = -1;

				pDeviceContext->Map(m_pSortConstantsBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &sortConstantsDst);
				*((SortConstants*)sortConstantsDst.pData) = sortConstantsSrc;
				pDeviceContext->Unmap(m_pSortConstantsBuffer, 0);
			}
			pDeviceContext->CSSetConstantBuffers(0, 1, &m_pSortConstantsBuffer);

			pDeviceContext->Dispatch(numThreadGroups, 1, 1);
		}

		sortConstantsSrc.sortConstantsY = 1;
		
		// sort outer
		for (compareDistance = compareDistance >> 1; compareDistance > 1024; compareDistance = compareDistance >> 1)
		{
			// set constants
			{
				sortConstantsSrc.sortConstantsX = compareDistance;
				sortConstantsSrc.compareDistance = compareDistance;
				pDeviceContext->Map(m_pSortConstantsBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &sortConstantsDst);
				*((SortConstants*)sortConstantsDst.pData) = sortConstantsSrc;
				pDeviceContext->Unmap(m_pSortConstantsBuffer, 0);
			}

			pDeviceContext->CSSetConstantBuffers(0, 1, &m_pSortConstantsBuffer);

			pDeviceContext->Dispatch(numThreadGroups, 1, 1);

		}

		// sort inner
		pDeviceContext->CSSetShader(m_pCSSortInner, 0, 0);
		pDeviceContext->Dispatch(numThreadGroups, 1, 1);

	}

	ID3D11UnorderedAccessView* uavs[1] = { 0 };
	pDeviceContext->CSSetUnorderedAccessViews(0, 1, uavs, 0);


//#define VALIDATE_SORT
#ifdef VALIDATE_SORT
	pDeviceContext->CopyResource(m_pTEMPBuffer, m_pAliveBuffer);
	D3D11_MAPPED_SUBRESOURCE TEMP;
	pDeviceContext->Map(m_pTEMPBuffer, 0, D3D11_MAP_READ, 0, &TEMP);
	AliveData* sorted = (AliveData*)TEMP.pData;

	for (int i = 0; i < m_count - 1; i++)
	{
		if (sorted[i].depth < sorted[i + 1].depth)
		{
			std::cout << "Data not sorted!" << std::endl;
		}
	}

	pDeviceContext->Unmap(m_pTEMPBuffer, 0);
#endif

}


void ParticleEmitter::update(float dt, ID3D11DeviceContext* pDeviceContext, Camera* pCamera)
{

	m_time += dt;
	m_timer += dt;


	int numNewParticles = 0;
	while (m_timer > m_spawnData.spawnTime)
	{
		numNewParticles++;
		m_timer -= m_spawnData.spawnTime;
	}

	if (numNewParticles > 0)
		addParticle(numNewParticles, pDeviceContext);





	// set shader

	// update frame data
	{
		SimulateFrameData frameDataSrc;
		frameDataSrc.t = m_time;
		frameDataSrc.dt = dt;
		frameDataSrc.eye = pCamera->getPosition();
		//frameDataSrc.view = pCamera->getViewMatrix();

		D3D11_MAPPED_SUBRESOURCE frameDataDst;
		pDeviceContext->Map(m_pSimulateFrameDataBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &frameDataDst);

		*(SimulateFrameData*)frameDataDst.pData = frameDataSrc;

		pDeviceContext->Unmap(m_pSimulateFrameDataBuffer, 0);
	}

	pDeviceContext->CSSetShader(m_pCSSimulate, 0, 0);

	// set UAVs
	{
		ID3D11UnorderedAccessView* UAVs[3] = { m_pParticleUAV, m_pDeadUAV, m_pAliveUAV };
		UINT intCounts[3] = { (UINT)-1, (UINT)-1, (UINT)0 };
		pDeviceContext->CSSetUnorderedAccessViews(0, 3, UAVs, intCounts);
	}

	// set cBuffers
	{
		ID3D11Buffer* cBuffers[2] = { m_pConstantDataBuffer, m_pSimulateFrameDataBuffer };
		pDeviceContext->CSSetConstantBuffers(0, 2, cBuffers);
	}

	pDeviceContext->Dispatch(ceil(m_spawnData.maxCount / 1024.0f), 1, 1);


	ID3D11UnorderedAccessView* null[3] = { 0, 0,0 };
	pDeviceContext->CSSetUnorderedAccessViews(0, 3, null, 0);


	// get particle count
	{
		pDeviceContext->CopyStructureCount(m_pCountBuffer, 0, m_pAliveUAV);

		D3D11_MAPPED_SUBRESOURCE countSrc;
		pDeviceContext->Map(m_pCountBuffer, 0, D3D11_MAP_READ, 0, &countSrc);
		m_count = *(unsigned int*)countSrc.pData;
		pDeviceContext->Unmap(m_pCountBuffer, 0);
	}


//#define SORT
#ifdef SORT
	if (m_count > 0)
	{
		sort(pDeviceContext);
	}
#endif
}

void ParticleEmitter::draw(ID3D11DeviceContext* pDeviceContext, Camera* pCamera)
{
	// set shaders
	pDeviceContext->VSSetShader(m_pVertexShader, 0, 0);
	pDeviceContext->GSSetShader(m_pGeometryShader, 0, 0);
	pDeviceContext->PSSetShader(m_pPixelShader, 0, 0);

	// set variables

	RenderData renderDataSrc;
	renderDataSrc.World = Matrix4x4f();
	renderDataSrc.View = pCamera->getViewMatrix();
	renderDataSrc.ViewInverse = pCamera->getTransformationMatrix();
	renderDataSrc.Projection = pCamera->getProjectionMatrix();
	renderDataSrc.maxLifeTime = m_spawnData.lifeTime;
	renderDataSrc.scale = m_spawnData.scale;
	renderDataSrc.maxV = Vector3f(m_constantData.maxXVelocity, m_constantData.maxYVelocity, m_constantData.maxZVelocity).getMagnitude();

	D3D11_MAPPED_SUBRESOURCE renderDataDst;
	pDeviceContext->Map(m_pConstantRenderBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &renderDataDst);
	*((RenderData*)renderDataDst.pData) = renderDataSrc;
	pDeviceContext->Unmap(m_pConstantRenderBuffer, 0);

	// set SRVs
	{
		ID3D11ShaderResourceView* SRVs[2] = { m_pParticleSRV, m_pAliveSRV };
		pDeviceContext->VSSetShaderResources(0, 2, SRVs);
	}

	pDeviceContext->VSSetConstantBuffers(0, 1, &m_pConstantRenderBuffer);

	pDeviceContext->GSSetConstantBuffers(0, 1, &m_pConstantRenderBuffer);

	pDeviceContext->PSSetShaderResources(0, 1, &m_pSpriteSRV);


	// draw

	ID3D11Buffer* vb = 0;
	UINT stride = 0;
	UINT offset = 0;
	pDeviceContext->IASetVertexBuffers(0, 1, &vb, &stride, &offset);
	pDeviceContext->IASetIndexBuffer(0, DXGI_FORMAT_UNKNOWN,0);


	pDeviceContext->DrawInstanced(m_count, 1, 0, 0);

	//pDeviceContext->Draw(m_count, 0);
}

int ParticleEmitter::getCount()
{
	return m_count;
}