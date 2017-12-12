#pragma once
#include <gbUtils/common.h>
#ifdef _MSC_VER
#include <Windows.h>
#endif
#include "thirdpartylib/glad/include/glad/glad.h"
namespace gb
{
    namespace render
    {
	class device;
	class gl
	{
		friend class device;
		static void APIENTRY _glDebugOutput(GLenum source, GLenum type, GLuint id, GLenum severity,
			GLsizei length, const GLchar *message, void *userParam);
	};
    };
};
