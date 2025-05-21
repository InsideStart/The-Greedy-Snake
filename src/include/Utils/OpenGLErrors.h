#pragma once
#ifndef ERRORS_H
#define ERRORS_H
#include <GL/glew.h>
#include <cstdlib>
#include <iostream>
#include <string>
using std::string;
class OpenGLErrors {
public:
    // 当GLSL代码编译失败时，显示OpenGL日志内容
    void printShaderLog(GLint shader);
    // 当OpenGL链接失败时，显示OpenGL日志内容
    void printProgramLog(GLint prog);
    // 检查OpenGL错误标志
    bool checkOpenGLError(string info = "");
    string OpenGLGetErrorString(GLenum errorCode);
};

#endif