#include "graphics.h"
#include "Window.h"

#include "Clock.h"
#include "ParticleEmitter.h"

#include <iostream>
#include "Camera.h"

int main()
{

	Graphics graphics;


	Window window;
	{
		WindowDescription descr
		{
			1600,
			1200,
			100,
			100,

			WindowStyle::FULLSCREEN/*WindowStyle::TITLEBAR | WindowStyle::CLOSE | WindowStyle::MINBOX | WindowStyle::MAXBOX*/
		};

		window.create(descr);
	}


	initGraphics(&window, &graphics);


	ConstantData constantData;
	constantData.maxXVelocity =		1200.0f;
	constantData.minXVelocity = -	1200.0f;
	constantData.maxYVelocity =		1200.0f;
	constantData.minYVelocity = -	1200.0f;
	constantData.maxZVelocity =		1200.0f;
	constantData.minZVelocity = -	1200.0f;
	constantData.xScale = 1.5f;
	constantData.yScale = 1.5f;
	constantData.zScale = 1.5f;
	constantData.timeScale = 15.5f;
	constantData.seedX = rand();
	constantData.seedY = rand();
	constantData.seedZ = rand();


	SpawnData spawnData;
	spawnData.maxCount = 1024 * 1024 * 1;
	spawnData.spawnTime = 1 / 500000.0f;
	spawnData.lifeTime = 2.0f;
	spawnData.spawnRadiusX = 90.0f;
	spawnData.spawnRadiusY = 90.0f;
	spawnData.spawnRadiusZ = 90.0f;
	spawnData.scale = 1.0f;


	ParticleEmitter emitter(constantData, spawnData, graphics.pDevice, graphics.pDeviceContext);



	Camera camera;
	camera.setPosition(Vector3f(0, 0, -10.0f));
	Clock clock;


	bool running = true;;
	bool alt = false;
	bool lButton = false;
	bool mButton = false;
	Vector2i lastMouse;

	float time = 0;
	float fpsCounter = 0;

	float dt;
	while (running)
	{
		clock.startNewFrame();
		dt = clock.getElapsedTime();
		Event event;

		while (window.pollEvent(event))
		{
			switch (event.type)
			{
			case Event::MouseWheel:
				camera.moveLocal(Vector3f(0, 0, event.mouseWheelDelta / 25.0f));
				break;
			case Event::KeyDown:
				if (event.key == Event::ALT)
					alt = true;
				break;
			case Event::KeyUp:
				if (event.key == Event::ALT)
					alt = false;
				break;
			case Event::MouseDown:
				if (event.mouseButton == 0)
					lButton = true;
				if (event.mouseButton == 2)
					mButton = true;
				lastMouse = event.mousePos;
				break;
			case Event::MouseUp:
				if (event.mouseButton == 0)
					lButton = false;
				if (event.mouseButton == 2)
					mButton = false;
				break;
			case Event::MouseMove:
			{
				Vector2i delta = lastMouse - event.mousePos;
				if (alt)
				{
					if (lButton)
					{
						camera.rotate(Quaternion(Vector3f(0, 1, 0), delta.x() * 3.14 / 180.0f));
						camera.rotateLocal(Quaternion(Vector3f(1, 0, 0), -delta.y() * 3.14 / 180.0f));
					}


					if (mButton)
					{
						camera.moveLocal(Vector3f(-delta.x() / 10.0f, -delta.y() / 20.0f, 0));
					}

				}
				lastMouse = event.mousePos;
				break;
			}
			case Event::Quit:
				running = false;;
			}
		}

		
		emitter.update(dt, graphics.pDeviceContext, &camera);


		float clearColor[] = { 0, 0, 0, 1 };
		clear(&graphics, clearColor);

		
		emitter.draw(graphics.pDeviceContext, &camera);


		present(&graphics);

		time += dt;
		fpsCounter++;
		if (time > 1)
		{
			std::cout << "Num Particles: " << emitter.getCount() << " FPS: " << fpsCounter << std::endl;
			time = 0;
			fpsCounter = 0;
		}

	}

	graphics.pBackBuffer->Release();
	graphics.pDepthStencil->Release();
	graphics.pDevice->Release();
	graphics.pDeviceContext->Release();
	graphics.pSwapChain->Release();

	

	std::cout << "test" << std::endl;

	return 0;
}
