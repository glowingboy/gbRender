#pragma once
#include <gbUtils/common.h>
#ifdef _MSC_VER
#include <Windows.h>
#endif
#include <glad/include/glad/glad.h>
#include "RenderNS.h"

GB_RENDER_NS_BEGIN

class Device;
class GL
{
	friend class Device;
	static void APIENTRY _glDebugOutput(GLenum source, GLenum type, GLuint id, GLenum severity,
		GLsizei length, const GLchar *message, void *userParam);
};

GB_RENDER_NS_END
