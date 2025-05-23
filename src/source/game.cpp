#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include <chrono>
#include <thread>
using std::string;
#include "Game/Map.h"
#include "Utils/OpenGLErrors.h"
const GLint frame = 1;
void init(GLFWwindow* window);
void display(GLFWwindow *window,Map& map);
int main(int argc, char **argv)
{
    if (!glfwInit())
    {
        // 发个日志显示glfw没有初始化
        std::cerr << "glfw init failed." << std::endl;
        exit(EXIT_FAILURE);
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(800, 800, "opengl-snake", NULL, NULL);
    if(!window) {
        std::cerr << "Create window failed." << std::endl;
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK)
    {
        std::cerr << "GLEW initialize failed." << std::endl;
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    Map map;
    OpenGLErrors openError;

    glfwSetWindowUserPointer(window, &map);
    glfwSetFramebufferSizeCallback(window, Map::framebuffer_size_callback);

    glfwSwapInterval(0);

    map.initMap();
    GLdouble updateInterval = 0.2;
    GLdouble renderInterval = 1.0 / 60.0;
    GLdouble lastUpdateTime = glfwGetTime();
    GLdouble lastRenderTime = glfwGetTime();
    // 主循环
    while(!glfwWindowShouldClose(window)) {
        
        glfwPollEvents();
        
        GLdouble currentTime = glfwGetTime();
        // 执行控制操作
        if(currentTime - lastUpdateTime >= updateInterval) {
            display(window, map);
            lastUpdateTime = currentTime;
        }
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        openError.checkOpenGLError("before render.");
        map.render();
        openError.checkOpenGLError("after render.");

        glfwSwapBuffers(window);
        GLdouble frameTime = currentTime + renderInterval;
        while (glfwGetTime() < frameTime)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
    }
    glfwTerminate();
    return 0;
}


void init(GLFWwindow *window)
{
}
void display(GLFWwindow *window,Map& map)
{

    // 添加按键检测
    glfwSetKeyCallback(window, Map::keyboard_input);
    // 进行上下左右控制
    map.updateSnake();
}
