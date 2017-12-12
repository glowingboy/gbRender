#pragma once
#include <gbUtils/common.h>
#include <gbUtils/string.h>
#ifdef _MSC_VER
#include <windows.h>
#endif

namespace gb
{
    namespace render
    {
	class device
	{
	    GB_SINGLETON(device);

	public:
	    bool Initialize();
		void Update();
#ifdef _MSC_VER
	private:
		static HGLRC _glContext;
		static HDC _hDC;
	private:
	    static LRESULT CALLBACK _windowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	    static bool _createGLContext(const HWND hWnd);
#endif
	};
	
    };
    
};

