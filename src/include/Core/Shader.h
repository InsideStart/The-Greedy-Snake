#pragma once
#ifndef SHADER_H
#define SHADER_H
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GLM/glm.hpp>
#include "Utils/Utils.h"
#include "Utils/OpenGLErrors.h"
class Shader {
public:
    Shader();
    void initShader();
    void createShaderProgram(const char* vp,const char* fp);
    void use();
    void setMat4(string uniformMat4, glm::mat4 mat4);
    GLuint getProgram();
    ~Shader();

private:
    GLuint vShader;
    GLuint fShader;
    GLuint resProgram;
    OpenGLErrors errors;
};

#endif