#include "Device.h"
#include <gbUtils/string.h>
#include "Director.h"
#include <wglext.h>
#include <gbUtils/logger.h>
using namespace gb::render;
using gb::utils::string;
using gb::utils::logger;
#ifdef _MSC_VER
HDC Device::_hDC;
HGLRC Device::_glContext;
#endif

Device::Device():
	_bInitialized(false),
	_bFullScreen(false)
{

}

bool Device::Initialize(const gb::physics::vec2<uint32>& screenSize)
{
	if (_bInitialized)
	{
		logger::Instance().warning("Device has been Initialized");
		return false;
	}
#ifdef _MSC_VER
	WNDCLASS wc;
	::memset(&wc, 0, sizeof(WNDCLASS));
	wc.lpfnWndProc = _windowProc;
	wc.hInstance = ::GetModuleHandle(NULL);
	wc.hbrBackground = (HBRUSH)(COLOR_BACKGROUND);
	wc.lpszClassName = TEXT("gbRender");
	wc.style = CS_OWNDC;

	//fixed cursor always is busy state issue.
	wc.hCursor = ::LoadCursor(NULL, IDC_ARROW);

	if (!::RegisterClass(&wc))
	{
		MessageBox(NULL, TEXT("device::Initialize Registerclass failed"), TEXT("gbRender"), MB_OK | MB_ICONERROR);
		return false;
	}

	if (screenSize.x != 0 && screenSize.y != 0)
	{
		_bFullScreen = true;
		_screenSize = screenSize;
	}
	else
	{
		_bFullScreen = false;
		_screenSize.x = ::GetSystemMetrics(SM_CXSCREEN);
		_screenSize.y = ::GetSystemMetrics(SM_CYSCREEN);
	}

	RECT rect;
	rect.left = 0;
	rect.top = 0;

	rect.right = _screenSize.x;
	rect.bottom = _screenSize.y;

	DWORD dwStyle =  WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;
	if (!_bFullScreen)
		dwStyle = WS_POPUP;
	::AdjustWindowRectEx(&rect, dwStyle, FALSE, 0);
	const HWND hWnd = ::CreateWindow(wc.lpszClassName, TEXT("gbRender"), dwStyle, 0, 0, rect.right - rect.left, rect.bottom - rect.top, 0, 0, wc.hInstance, 0);

	if (hWnd == NULL)
		return false;

	::ShowWindow(hWnd, SW_SHOW);

	_bInitialized = true;
	return true;
#endif
}

#ifdef _MSC_VER
LRESULT CALLBACK Device::_windowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	static bool bShouldGetCursorPos = false;

	switch (uMsg)
	{
	case WM_CREATE:
		if (!_createGLContext(hWnd))
			::PostQuitMessage(1);
		break;
	case WM_PAINT:
		RECT rect;
		GetClientRect(hWnd, &rect);
		ValidateRect(hWnd, &rect);
		break;
	case WM_KEYDOWN:
		bShouldGetCursorPos = true;
		//gbInput::SetKeyState(wParam, true);
		break;
	case WM_LBUTTONDOWN:
		bShouldGetCursorPos = true;
		//gbInput::SetKeyState(VK_LBUTTON, true);
		break;
	case WM_RBUTTONDOWN:
		bShouldGetCursorPos = true;
		//gbInput::SetKeyState(VK_RBUTTON, true);
		break;
	case WM_KEYUP:
		bShouldGetCursorPos = true;
		//gbInput::SetKeyState(wParam, false);
		break;
	case WM_LBUTTONUP:
		bShouldGetCursorPos = true;
		//gbInput::SetKeyState(VK_LBUTTON, false);
		break;
	case WM_RBUTTONUP:
		bShouldGetCursorPos = true;
		//gbInput::SetKeyState(VK_RBUTTON, false);
		break;
	case WM_MOUSEMOVE:
		bShouldGetCursorPos = true;
		//gbInput::SetKeyState(gb_MS_MOVE, true);
		break;
	case WM_DESTROY:
		wglDeleteContext(_glContext);
		PostQuitMessage(0);
		break;
	}

	if (bShouldGetCursorPos)
	{
		POINT point;
		BOOL ret = ::GetCursorPos(&point);
		assert(ret);

		ret = ::ScreenToClient(hWnd, &point);
		assert(ret);
		// gbInput::_cursorPos.X = point.x;
		// gbInput::_cursorPos.Y = point.y;
		bShouldGetCursorPos = false;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

bool Device::_createGLContext(const HWND hWnd)
{
	PIXELFORMATDESCRIPTOR pfd =
	{
		sizeof(PIXELFORMATDESCRIPTOR),
		1,
		PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
		PFD_TYPE_RGBA,
		32,
		0, 0, 0, 0, 0, 0,
		0,
		0,
		0,
		0, 0, 0, 0,
		24,//DEPTH
		8,//STENCIL
		0,
		0,
		0,0,0,0
	};
	_hDC = GetDC(hWnd);
	if (!_hDC)
	{
		MessageBox(NULL, TEXT("GetDC ERROR"), TEXT("gbGL"), MB_OK | MB_ICONERROR);
		return false;
	}
	int PixelFormat = ChoosePixelFormat(_hDC, &pfd);
	if (!PixelFormat)
	{
		MessageBox(NULL, TEXT("ChoosePixelFormat ERROR"), TEXT("gbGL"), MB_OK | MB_ICONERROR);
		return false;
	}
	if (!SetPixelFormat(_hDC, PixelFormat, &pfd))
	{
		MessageBox(NULL, TEXT("SetPixelFormat ERROR"), TEXT("gbGL"), MB_OK | MB_ICONERROR);
		return false;
	}


	HGLRC dummyRC = wglCreateContext(_hDC);
	if (!dummyRC)
	{
		MessageBox(NULL, TEXT("dummyRC ERROR"), TEXT("gbGL"), MB_OK | MB_ICONERROR);
		return false;
	}
	if (!wglMakeCurrent(_hDC, dummyRC))
	{
		MessageBox(NULL, TEXT("wglMakeCurrent ERROR"), TEXT("gbGL"), MB_OK | MB_ICONERROR);
		return false;
	}
	PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB = (PFNWGLCREATECONTEXTATTRIBSARBPROC)wglGetProcAddress("wglCreateContextAttribsARB");
	if (wglCreateContextAttribsARB == nullptr)
	{
		MessageBox(NULL, TEXT("wglGetProcAddress wglCreateContextAttribsARB ERROR"), TEXT("gbGL"), MB_OK | MB_ICONERROR);
		return false;
	}

	const int attribList[] =
	{
		WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_DEBUG_BIT_ARB,
	0
	};

	_glContext = wglCreateContextAttribsARB(_hDC, nullptr, attribList);
	if (!_glContext)
	{
		MessageBox(NULL, TEXT("finalRC ERROR"), TEXT("gbGL"), MB_OK | MB_ICONERROR);
		return false;
	}
	wglDeleteContext(dummyRC);

	if (!wglMakeCurrent(_hDC, _glContext))
	{
		MessageBox(NULL, TEXT("wglMakeCurrent ERROR"), TEXT("gbGL"), MB_OK | MB_ICONERROR);
		return false;
	}

	if (!gladLoadGL())
	{
		MessageBox(NULL, TEXT("gladLoadGL ERROR"), TEXT("gbGL"), MB_OK | MB_ICONERROR);
		return false;
	}

	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glDebugMessageCallback((GLDEBUGPROC)&(GL::_glDebugOutput), nullptr);
	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);

	//   std::string supportedExt((const char*)glGetString(GL_EXTENSIONS));
	//   bool bSupport = true;
	//   for each (std::string dmdExt in dmdGLExtension)
	   //     {
	   //	 if (supportedExt.find(dmdExt, 0) == std::string::npos)
	   //	 {
	   //	     bSupport = false;
	   //	     gbLog::Instance().Error(gbString("opengl not unsupport @") + dmdExt.c_str());
	   //	 }
	   //     }
	//   if (!bSupport)
	   //::PostQuitMessage(0);
	return true;
}
#endif

bool Device::Update()const
{
#ifdef _WIN32
	MSG msg;
	if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
		if (msg.message == WM_QUIT)
			return false;
	}
	else
	{
		SwapBuffers(_hDC);
	}
	return true;
#endif
}
