#include "Utils/OpenGLErrors.h"

void OpenGLErrors::printShaderLog(GLint shader)
{
    int len = 0;
    int chWrittn = 0;
    char *log;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);
    if (len > 0)
    {
        log = (char *)malloc(len);
        glGetShaderInfoLog(shader, len, &chWrittn, log);
        std::cout << "Shader Info Log: " << log << std::endl;
        free(log);
    }
}

void OpenGLErrors::printProgramLog(GLint prog)
{
    int len = 0;
    int chWrittn = 0;
    char *log;
    glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &len);
    if (len > 0)
    {
        log = (char *)malloc(len);
        glGetProgramInfoLog(prog, len, &chWrittn, log);
        std::cout << "Program Info Log: " << log << std::endl;
        free(log);
    }
}

bool OpenGLErrors::checkOpenGLError(string info)
{
    bool foundError = false;
    int glErr = glGetError();
    while (glErr != GL_NO_ERROR)
    {
        if(info.empty())
            info = "glError";
        std::cout << info << " : " << OpenGLGetErrorString(glErr) << std::endl;
        foundError = true;
        glErr = glGetError();
    }
    return foundError;
}

string OpenGLErrors::OpenGLGetErrorString(GLenum errorCode)
{
    switch (errorCode)
    {
    case GL_INVALID_ENUM:
        return "GL_INVALID_ENUM: 枚举参数无效";
    case GL_INVALID_VALUE:
        return "GL_INVALID_VALUE: 数值参数越界";
    case GL_INVALID_OPERATION:
        return "GL_INVALID_OPERATION: 当前操作非法";
    case GL_STACK_OVERFLOW:
        return "GL_STACK_OVERFLOW: 矩阵堆栈溢出";
    case GL_STACK_UNDERFLOW:
        return "GL_STACK_UNDERFLOW: 矩阵堆栈下溢";
    case GL_OUT_OF_MEMORY:
        return "GL_OUT_OF_MEMORY: 内存不足";
    case GL_INVALID_FRAMEBUFFER_OPERATION:
        return "GL_INVALID_FRAMEBUFFER_OPERATION: 帧缓冲区不完整";
    default:
        return "未知 OpenGL 错误 (" + std::to_string(errorCode) + ")";
    }
}