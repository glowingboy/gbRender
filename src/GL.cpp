#include "GL.h"
#include <glad/src/glad.c>
#include <gbUtils/string.h>
#include <gbUtils/logger.h>
#include "data/Shader.h"

using namespace gb::render;
using gb::utils::string;
using gb::utils::logger;

#ifdef _MSC_VER
#pragma comment(lib,"opengl32.lib")
#pragma comment(lib,"glu32.lib")
#endif

const data::Shader* GL::_currentShader = nullptr;

void GL::applyShader(const data::Shader* shader)
{
	if (_currentShader != shader)
	{
		_currentShader = shader;

		_currentShader->Use();

		//gl settings
	}
}
void APIENTRY GL::_glDebugOutput(GLenum source, GLenum type, GLuint id, GLenum severity,
	GLsizei length, const GLchar *message, void *userParam)
{
	// ignore non-significant error/warning codes
	if (id == 131169 || id == 131185 || id == 131218 || id == 131204 || id == 131184) return;


	string msgSrc;
	switch (source)
	{
	case GL_DEBUG_SOURCE_API:             msgSrc = "Source: API"; break;
	case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   msgSrc = "Source: Window System"; break;
	case GL_DEBUG_SOURCE_SHADER_COMPILER: msgSrc = "Source: Shader Compiler"; break;
	case GL_DEBUG_SOURCE_THIRD_PARTY:     msgSrc = "Source: Third Party"; break;
	case GL_DEBUG_SOURCE_APPLICATION:     msgSrc = "Source: Application"; break;
	case GL_DEBUG_SOURCE_OTHER:           msgSrc = "Source: Other"; break;
	}
	string msgType;

	bool bErrorType = false;
	switch (type)
	{
	case GL_DEBUG_TYPE_ERROR:               msgType = "Type: Error"; bErrorType = true; break;
	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: msgType = "Type: Deprecated Behaviour"; break;
	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  msgType = "Type: Undefined Behaviour"; break;
	case GL_DEBUG_TYPE_PORTABILITY:         msgType = "Type: Portability"; break;
	case GL_DEBUG_TYPE_PERFORMANCE:         msgType = "Type: Performance"; break;
	case GL_DEBUG_TYPE_MARKER:              msgType = "Type: Marker"; break;
	case GL_DEBUG_TYPE_PUSH_GROUP:          msgType = "Type: Push Group"; break;
	case GL_DEBUG_TYPE_POP_GROUP:           msgType = "Type: Pop Group"; break;
	case GL_DEBUG_TYPE_OTHER:               msgType = "Type: Other"; break;
	}

	string msgSeverity;
	switch (severity)
	{
	case GL_DEBUG_SEVERITY_HIGH:         msgSeverity = "Severity: high"; break;
	case GL_DEBUG_SEVERITY_MEDIUM:       msgSeverity = "Severity: medium"; break;
	case GL_DEBUG_SEVERITY_LOW:          msgSeverity = "Severity: low"; break;
	case GL_DEBUG_SEVERITY_NOTIFICATION: msgSeverity = "Severity: notification"; break;
	}

	if (bErrorType)
	{
		logger::Instance().error(string("OpenGL debug message, id@") + id + ", msg@" + message);
		logger::Instance().error(msgSrc + ", " + msgType + ", " + msgSeverity);
		//assert(0);
		::system("pause");
	}
	else
	{
		logger::Instance().log(string("OpenGL debug message, id@") + id + ", msg@" + message);
		logger::Instance().log(msgSrc + ", " + msgType + ", " + msgSeverity);
	}
}

GL::Sync::Sync():
	_sync(0)
{}

GL::Sync::~Sync()
{
	if (_sync != 0)
		glDeleteSync(_sync);
}
void GL::Sync::Set()
{
	if (_sync != 0)
		glDeleteSync(_sync);
	_sync = glFenceSync(GL_SYNC_GPU_COMMANDS_COMPLETE, 0);
}

void GL::Sync::Wait() const
{
	if (_sync)
	{
		while (true)
		{
			const GLenum waitReturn = glClientWaitSync(_sync, GL_SYNC_FLUSH_COMMANDS_BIT, 1);
			if (waitReturn == GL_ALREADY_SIGNALED || waitReturn == GL_CONDITION_SATISFIED)
				break;
		}
	}
}