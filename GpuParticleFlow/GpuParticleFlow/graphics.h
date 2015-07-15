#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "D3D11.h"

class Window;


struct Graphics
{
	ID3D11Device* pDevice;
	ID3D11DeviceContext* pDeviceContext;
	IDXGISwapChain* pSwapChain;
	ID3D11RenderTargetView* pBackBuffer;
	ID3D11DepthStencilView* pDepthStencil;

};

void initGraphics(Window* window, Graphics* pGraphics);

void clear(Graphics* pGraphics, float* color);
void present(Graphics* pGraphics);


bool CompileShader(LPCWSTR srcFile, LPCSTR entryPoint, LPCSTR profile, ID3D11Device* pDevice, ID3DBlob** ppBlob);

void GetNoiseMap(int width, int height, ID3D11Device* pDevice, ID3D11Texture2D** ppTexture);


#endif