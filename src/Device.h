#pragma once
#include <gbUtils/common.h>
#include <gbUtils/string.h>
#ifdef _MSC_VER
#include <windows.h>
#endif
#include "RenderNS.h"
#include "Type.h"
#include <gbPhysics/type.h>

GB_RENDER_NS_BEGIN
	class Device
	{
		GB_SINGLETON_NO_CTORDEF(Device);
	public:
	    bool Initialize(const gb::physics::vec2<uint32>& screenSize);
		bool Update()const;


		inline const gb::physics::vec2<uint32>& GetScreenSize()const
		{
			return _screenSize;
		}

		inline bool IsInitialized()const { return _bInitialized; }

	private:
		gb::physics::vec2<uint32> _screenSize;
		bool _bInitialized;
		bool _bFullScreen;
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

