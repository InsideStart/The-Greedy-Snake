#include "Core/Shader.h"
#include <iostream>
#include <string>
using std::string;
Shader::Shader() {
    initShader();
}

Shader::~Shader() {
    glDeleteShader(vShader);
    glDeleteShader(fShader);
}
void Shader::setMat4(string uniformMat4, glm::mat4 mat4) {
    GLint loc = glGetUniformLocation(resProgram, uniformMat4.c_str());
    if(loc == -1) {
        std::cerr << "Uniform " << uniformMat4 << " not found!" << std::endl;
        return;
    }
    glUniformMatrix4fv(loc, 1, GL_FALSE, &mat4[0][0]);
}
void Shader::initShader()
{
    // 1. create shader
    vShader = glCreateShader(GL_VERTEX_SHADER);
    fShader = glCreateShader(GL_FRAGMENT_SHADER);
    // 2. bind data
}

void Shader::createShaderProgram(const char *vp, const char *fp) {
    const char *vShaderSource,*fShaderSource;
    Utils util;

    string vSource = util.readShaderSource(vp);
    vShaderSource = vSource.c_str();
    string fSource = util.readShaderSource(fp);
    fShaderSource = fSource.c_str();

    glShaderSource(vShader, 1, &vShaderSource, NULL);
    glShaderSource(fShader, 1, &fShaderSource, NULL);

    glCompileShader(vShader);
    glCompileShader(fShader);
    errors.printShaderLog(vShader);
    errors.printShaderLog(fShader);

    resProgram = glCreateProgram();
    glAttachShader(resProgram, vShader);
    glAttachShader(resProgram, fShader);

    glLinkProgram(resProgram);
}

GLuint Shader::getProgram() {
    return resProgram;
}

void Shader::use() {
    glUseProgram(resProgram);
}