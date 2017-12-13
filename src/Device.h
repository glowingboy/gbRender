#pragma once
#include <gbUtils/common.h>
#include <gbUtils/string.h>
#ifdef _MSC_VER
#include <windows.h>
#endif
#include "RenderNS.h"

GB_RENDER_NS_BEGIN
	class Device
	{
	    GB_SINGLETON(Device);

	public:
	    bool Initialize();
		bool Update()const;
#ifdef _MSC_VER
	private:
		static HGLRC _glContext;
		static HDC _hDC;
	private:
	    static LRESULT CALLBACK _windowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	    static bool _createGLContext(const HWND hWnd);
#endif
	};

GB_RENDER_NS_END

