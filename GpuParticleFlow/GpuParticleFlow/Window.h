#pragma once

#include <Windows.h>
#include "Vector2.h"

struct Event
{
	// ****************************************************
	// @brief what type of event
	// ****************************************************
	enum EventType
	{
		Undefined = 0,
		Quit,
		MouseMove,
		MouseDown,
		MouseUp,
		MouseWheel,
		KeyDown,
		KeyUp
	} type;


	Vector2i mousePos;
	unsigned short mouseButton;
	int mouseWheelDelta;

	enum Key
	{
		ALT,

	} key;
};

// ****************************************************
// @brief holds window informations
// ****************************************************
struct WindowDescription
{
	int width;		// width of the window
	int height;		// height of the window
	int positionX;	// start position x of the window
	int positionY;	// start position y of the window

	
	unsigned int style; // window style, see Engine::Core::WindowStyle
};

namespace WindowStyle
{
	enum
	{
		NONE = 0,
		TITLEBAR = 2,
		RESIZE = 4,
		CLOSE = 8,
		MINBOX = 16,
		MAXBOX = 32,
		FULLSCREEN = 64,

		DEFAULT = TITLEBAR | RESIZE | CLOSE | MINBOX | MAXBOX
	};


}

class Window
{
public:

	Window();
	Window(WindowDescription desc);
	~Window();

	void create(WindowDescription& desc);
	void close();

	bool pollEvent(Event& event);

	HWND getHandle();

	const WindowDescription& getDescription() const;

	bool isOpen();

private:

	bool m_Open;
	bool m_Visible;

	WindowDescription m_Desc;

	HWND m_Wnd;								
	HINSTANCE m_Instance;					


};
