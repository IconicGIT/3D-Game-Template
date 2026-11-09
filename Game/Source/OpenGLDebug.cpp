#include "OpenGLDebug.h"

#include "Glew/include/GL/glew.h"
#include "Log.h"

static const char* GetOpenGLErrorName(GLenum error)
{
    switch (error)
    {
    case GL_INVALID_ENUM:
        return "GL_INVALID_ENUM";

    case GL_INVALID_VALUE:
        return "GL_INVALID_VALUE";

    case GL_INVALID_OPERATION:
        return "GL_INVALID_OPERATION";

    case GL_INVALID_FRAMEBUFFER_OPERATION:
        return "GL_INVALID_FRAMEBUFFER_OPERATION";

    case GL_OUT_OF_MEMORY:
        return "GL_OUT_OF_MEMORY";

    default:
        return "UNKNOWN_OPENGL_ERROR";
    }
}

void CheckOpenGLError(const char* operation)
{
    GLenum error;

    // Puede haber varios errores pendientes.
    while ((error = glGetError()) != GL_NO_ERROR)
    {
        DEBUG_LOG(
            "OpenGL error after [%s]: %s (0x%04X)",
            operation,
            GetOpenGLErrorName(error),
            static_cast<unsigned int>(error)
        );
    }
}