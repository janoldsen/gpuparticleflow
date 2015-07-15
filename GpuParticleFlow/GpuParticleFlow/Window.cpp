#include "Window.h"
#include <windowsx.h>
#include <iostream>

Window::Window()
{
	m_Open = false;
	m_Visible = false;
}


Window::Window(WindowDescription desc)
{

	// if the window is already open, close the old window and create a new one
	if (m_Open)
	{
		close();
	}

	create(desc);
}


Window::~Window()
{
	std::cout << "shutting down windows" << std::endl;

	// if the window is still open close it
	if (m_Open)
	{
		close();
	}

	std::cout << "windows shutdown done" << std::endl;
}

const WindowDescription& Window::getDescription() const
{
	return m_Desc;
}




static Event::Key winKeyToEngineKey(unsigned int winKey)
{
	switch (winKey)
	{
	case VK_MENU:
		return Event::ALT;

	default:
		break;
	}

}

bool Window::pollEvent(Event& event)
{
	MSG message;
	if (PeekMessage(&message, NULL, 0, 0, PM_REMOVE)) // get windows message
	{
		// translate message to event structure
		switch (message.message)
		{
		case WM_QUIT:
			event.type = Event::Quit;
			break;
		case WM_MOUSEMOVE:
			event.type = Event::MouseMove;
			event.mousePos.x() = GET_X_LPARAM(message.lParam);
			event.mousePos.y() = GET_Y_LPARAM(message.lParam);
			break;
		case WM_LBUTTONDOWN:
			event.type = Event::MouseDown;
			event.mouseButton = 0;
			event.mousePos.x() = GET_X_LPARAM(message.lParam);
			event.mousePos.y() = GET_Y_LPARAM(message.lParam);
			break;
		case WM_RBUTTONDOWN:
			event.type = Event::MouseDown;
			event.mouseButton = 1;
			event.mousePos.x() = GET_X_LPARAM(message.lParam);
			event.mousePos.y() = GET_Y_LPARAM(message.lParam);
			break;
		case WM_MBUTTONDOWN:
			event.type = Event::MouseDown;
			event.mouseButton = 2;
			event.mousePos.x() = GET_X_LPARAM(message.lParam);
			event.mousePos.y() = GET_Y_LPARAM(message.lParam);
			break;
		case WM_LBUTTONUP:
			event.type = Event::MouseUp;
			event.mouseButton = 0;
			event.mousePos.x() = GET_X_LPARAM(message.lParam);
			event.mousePos.y() = GET_Y_LPARAM(message.lParam);
			break;
		case WM_RBUTTONUP:
			event.type = Event::MouseUp;
			event.mouseButton = 1;
			event.mousePos.x() = GET_X_LPARAM(message.lParam);
			event.mousePos.y() = GET_Y_LPARAM(message.lParam);
			break;
		case WM_MBUTTONUP:
			event.type = Event::MouseUp;
			event.mouseButton = 2;
			event.mousePos.x() = GET_X_LPARAM(message.lParam);
			event.mousePos.y() = GET_Y_LPARAM(message.lParam);
			break;
		case WM_KEYDOWN:
		case WM_SYSKEYDOWN:
			event.type = Event::KeyDown;
			event.key = winKeyToEngineKey(message.wParam);
			break;
		case WM_KEYUP:
		case WM_SYSKEYUP:
			event.type = Event::KeyUp;
			event.key = winKeyToEngineKey(message.wParam);
			break;
		case WM_MOUSEWHEEL:
			event.type = Event::MouseWheel;
			event.mouseWheelDelta = GET_WHEEL_DELTA_WPARAM(message.wParam);
			event.mousePos.x() = GET_X_LPARAM(message.lParam);
			event.mousePos.y() = GET_Y_LPARAM(message.lParam);
			break;
		default:
			event.type = Event::Undefined;
			break;
		}

		// send message to window
		TranslateMessage(&message);
		DispatchMessage(&message);

		return true;
	}
	else // if there is no message on the queue return false
	{
		return false;
	}

}

bool Window::isOpen()
{
	return m_Open;
}

HWND Window::getHandle()
{
	return m_Wnd;
}

LRESULT CALLBACK WndProc(HWND hwnd, unsigned int message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_CLOSE:
		PostQuitMessage(0);
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}

	return DefWindowProc(hwnd, message, wParam, lParam);
}

void Window::create(WindowDescription& desc)
{

	m_Open = true;
	m_Desc = desc;

	WNDCLASS wc;

	// window title in wchar
	//std::wstring title = std::wstring(desc.title.begin(), desc.title.end());

	//fill windows window class from the window description
	wc.cbClsExtra = NULL;
	wc.cbWndExtra = NULL;
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hIcon = NULL;
	wc.hInstance = m_Instance;
	wc.lpfnWndProc = WndProc;
	wc.lpszClassName = L"blah";
	wc.lpszMenuName = NULL;
	wc.style = CS_VREDRAW | CS_HREDRAW;

	RegisterClass(&wc);

	// translate platform independent style to windows style
	unsigned int style = WS_VISIBLE;

	if (desc.style & WindowStyle::TITLEBAR)
		style |= WS_CAPTION;
	if (desc.style & WindowStyle::RESIZE)
		style |= WS_SIZEBOX;
	if (desc.style & WindowStyle::CLOSE)
		style |= WS_SYSMENU;
	if (desc.style & WindowStyle::MINBOX)
		style |= WS_MINIMIZEBOX;
	if (desc.style & WindowStyle::MAXBOX)
		style |= WS_MAXIMIZEBOX;

	// calculate window rectangle
	RECT rect = { desc.positionX, desc.positionY, desc.width + desc.positionX, desc.height + desc.positionY };
	AdjustWindowRect(&rect, style, false);

	//create the window
	if (!(desc.style & WindowStyle::FULLSCREEN))
	{
		m_Wnd = CreateWindow(wc.lpszClassName,
			wc.lpszClassName,
			style,
			rect.left,
			rect.top,
			rect.right - rect.left,
			rect.bottom - rect.top,
			NULL,
			NULL,
			m_Instance,
			NULL);
	}
	else
	{
		MONITORINFO mi = { sizeof(mi) };
		GetMonitorInfo(MonitorFromWindow(m_Wnd, MONITOR_DEFAULTTOPRIMARY), &mi);


		m_Wnd = CreateWindow(wc.lpszClassName,
			wc.lpszClassName,
			style | WS_POPUP,
			mi.rcMonitor.left,
			mi.rcMonitor.top,
			mi.rcMonitor.right - mi.rcMonitor.left,
			mi.rcMonitor.bottom - mi.rcMonitor.top,
			m_Wnd,
			NULL,
			m_Instance,
			NULL);


		//DWORD dwStyle = GetWindowLong(m_Wnd, GWL_STYLE);
		//dwStyle &= ~WS_OVERLAPPED;

		//SetWindowLong(m_Wnd, GWL_STYLE, dwStyle);


		//SetWindowPos(m_Wnd, HWND_TOP,
		//	mi.rcMonitor.left, mi.rcMonitor.top,
		//	mi.rcMonitor.right - mi.rcMonitor.left, mi.rcMonitor.bottom - mi.rcMonitor.top,
		//	SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
	}

}



void Window::close()
{
	//destroy the window
	DestroyWindow(m_Wnd);
}