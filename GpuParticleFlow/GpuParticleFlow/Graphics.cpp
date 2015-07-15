#include "graphics.h"

#include "Window.h"

#include <D3Dcompiler.h>
#include <D3DX11.h>

#include <iostream>


void initGraphics(Window* window, Graphics* pGraphics)
{
	// Description of the swap chain
	
	
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));
	swapChainDesc.BufferCount = 1;
	swapChainDesc.BufferDesc.Width = window->getDescription().width;
	swapChainDesc.BufferDesc.Height = window->getDescription().height;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.OutputWindow = window->getHandle();
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.Windowed = TRUE;

	// create device and swap chain


	D3D11CreateDeviceAndSwapChain(
		NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		NULL/*D3D11_CREATE_DEVICE_DEBUG*/,
		NULL,
		NULL,
		D3D11_SDK_VERSION,
		&swapChainDesc,
		&pGraphics->pSwapChain,
		&pGraphics->pDevice,
		NULL,
		&pGraphics->pDeviceContext);
	



	ID3D11Texture2D *pBackBuffer;
	pGraphics->pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);

	pGraphics->pDevice->CreateRenderTargetView(pBackBuffer, NULL, &pGraphics->pBackBuffer);
	pBackBuffer->Release();

	// set the render target as the back buffer
	pGraphics->pDeviceContext->OMSetRenderTargets(1, &pGraphics->pBackBuffer, NULL);



	D3D11_TEXTURE2D_DESC descDepth;
	descDepth.Width = window->getDescription().width;
	descDepth.Height = window->getDescription().height;
	descDepth.MipLevels = 1;
	descDepth.ArraySize = 1;
	//descDepth.Format = DXGI_FORMAT_D32_FLOAT_S8X24_UINT;
	descDepth.Format = DXGI_FORMAT_R32_TYPELESS;
	descDepth.SampleDesc.Count = 1;
	descDepth.SampleDesc.Quality = 0;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
	descDepth.CPUAccessFlags = 0;
	descDepth.MiscFlags = 0;

	ID3D11Texture2D *pDepthBuffer;

	pGraphics->pDevice->CreateTexture2D(&descDepth, NULL, &pDepthBuffer);


	// Create the depth stencil view

	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
	ZeroMemory(&descDSV, sizeof(descDSV));
	descDSV.Format = DXGI_FORMAT_D32_FLOAT;
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice = 0;

	pGraphics->pDevice->CreateDepthStencilView(pDepthBuffer, &descDSV, &pGraphics->pDepthStencil);
	pDepthBuffer->Release();

	// Bind the depth stencil view
	ID3D11RenderTargetView* backBufferView = pGraphics->pBackBuffer;
	pGraphics->pDeviceContext->OMSetRenderTargets(1, &pGraphics->pBackBuffer, pGraphics->pDepthStencil);


	// set depth stencil state
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	depthStencilDesc.DepthEnable = false;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
	depthStencilDesc.StencilEnable = false;

	ID3D11DepthStencilState* depthStencilState;
	pGraphics->pDevice->CreateDepthStencilState(&depthStencilDesc, &depthStencilState);
	pGraphics->pDeviceContext->OMSetDepthStencilState(depthStencilState, 0);


	// set blend mode
	{
		float blendFactor[] = { 0.0f, 0.0f, 0.0f, 0.0f };

		D3D11_BLEND_DESC desc;
		desc.AlphaToCoverageEnable = false;
		desc.IndependentBlendEnable = false;
		desc.RenderTarget[0].BlendEnable = true;
		desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ZERO;
		desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
		desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;


		ID3D11BlendState* blendState;

		pGraphics->pDevice->CreateBlendState(&desc, &blendState);

		pGraphics->pDeviceContext->OMSetBlendState(blendState, 0, 0xffffffff);
	}

	// set viewport
	D3D11_VIEWPORT vp;
	vp.Height = window->getDescription().height;
	vp.Width = window->getDescription().width;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	pGraphics->pDeviceContext->RSSetViewports(1, &vp);



	D3D11_RECT rect;
	rect.left = 0;
	rect.right = window->getDescription().width;
	rect.top = 0;
	rect.bottom = window->getDescription().height;

	//m_DeviceContext->RSSetScissorRects(1, &rect);

	ID3D11RasterizerState * pRasterState;

	D3D11_RASTERIZER_DESC rasterizerState;
	rasterizerState.FillMode = D3D11_FILL_SOLID; 
	rasterizerState.CullMode = D3D11_CULL_BACK;
	rasterizerState.FrontCounterClockwise = true;
	rasterizerState.DepthBias = 0;
	rasterizerState.DepthBiasClamp = 0;
	rasterizerState.SlopeScaledDepthBias = 0;
	rasterizerState.DepthClipEnable = true;
	rasterizerState.ScissorEnable = false;
	rasterizerState.MultisampleEnable = false;
	rasterizerState.AntialiasedLineEnable = false;

	pGraphics->pDevice->CreateRasterizerState(&rasterizerState, &pRasterState);


	pGraphics->pDeviceContext->RSSetState(pRasterState);

		
	pGraphics->pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);




}




void clear(Graphics* pGraphics, float* color)
{
	pGraphics->pDeviceContext->ClearRenderTargetView(pGraphics->pBackBuffer, color);
	pGraphics->pDeviceContext->ClearDepthStencilView(pGraphics->pDepthStencil, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

void present(Graphics* pGraphics)
{
	pGraphics->pSwapChain->Present(0, 0);
}




bool CompileShader(LPCWSTR srcFile, LPCSTR entryPoint, LPCSTR profile, ID3D11Device* pDevice, ID3DBlob** ppBlob)
{

	ID3DBlob* shaderBlob = nullptr;
	ID3DBlob* errorBlob = nullptr;




	HRESULT hr = D3DX11CompileFromFile(
		srcFile,
		NULL,
		NULL,
		entryPoint,
		profile,
		0,/*D3DCOMPILE_DEBUG | D3DCOMPILE_OPTIMIZATION_LEVEL0 | D3DCOMPILE_SKIP_OPTIMIZATION,*/
		0,
		0,
		&shaderBlob,
		&errorBlob,
		0
		);

	if (FAILED(hr))
	{
		std::cout << srcFile << std::endl << (char*)errorBlob->GetBufferPointer();
		errorBlob->Release();

		if (shaderBlob)
			shaderBlob->Release();

		return false;

	}

	*ppBlob = shaderBlob;
	return true;

}


static inline float getRandom()
{
	float random = (float)rand() / (float)RAND_MAX;
	return (random * 2) - 1;
}

void GetNoiseMap(int width, int height, ID3D11Device* pDevice, ID3D11Texture2D** ppTexture)
{
	D3D11_TEXTURE2D_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.Width = width;
	desc.Height = height;
	desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	desc.Usage = D3D11_USAGE_IMMUTABLE;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	desc.MipLevels = 1;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;

	float* values = new float[desc.Width*desc.Height * 4];
	float* ptr = values;
	for (int i = 0; i < desc.Width * desc.Height; i++)
	{
		ptr[0] = getRandom();
		ptr[1] = getRandom();
		ptr[2] = getRandom();
		ptr[3] = getRandom();

		ptr += 4;
	}

	D3D11_SUBRESOURCE_DATA data;
	data.pSysMem = values;
	data.SysMemPitch = desc.Width * 4 * sizeof(float);
	data.SysMemSlicePitch = 0;

	pDevice->CreateTexture2D(&desc, &data, ppTexture);

	delete[] values;



}