#pragma once
#ifndef MAP_H
#define MAP_H

#include <GL/glew.h>
#include <GL/glu.h>
#include <GLFW/glfw3.h>
#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include "Core/Shader.h"
#include "Utils/Utils.h"
#include "Utils/OpenGLErrors.h"
#include <vector>
#include <ctime>
#include <stdlib.h>
using std::pair;
using std::vector;
#pragma pack(push,1)
struct GridVertex
{
    glm::vec2 position;
    glm::vec3 color;
};
#pragma pack(pop)
enum SNAKE_DIRECTION {LEFT = 1, UP = 2, RIGHT = 3, DOWN = 4};
enum { EMPTY, SNAKE, FOOD };
const GLuint GRID_WIDTH = 40;
const GLuint GRID_HEIGHT = 40;
const GLfloat CELL_SIZE = 10.0f;
// 默认的蛇的颜色
const glm::vec3 SNAKE_COLOR = glm::vec3(0.235f,0.706f,0.392f);
const GLfloat SNAKE_SPEED = 1;
static SNAKE_DIRECTION direction = LEFT;
// 默认的食物的颜色
const glm::vec3 FOOD_COLOR = glm::vec3(1.0f,0.549f,0.0f);

class Map
{
public:
    Map();
    void initData();
    void initMap();

    // 绑定属性
    void setVertexAttributePointers();
    void reshape(int width, int height);
    void render();
    static void framebuffer_size_callback(GLFWwindow *window, int width, int height);

    // 更新顶点数据
    void updateVertexData();
    // 处理键盘输入
    static void keyboard_input(GLFWwindow *window, int key,int scancode, int action, int mods);
    // 蛇移动
    void updateSnake();
    ~Map();
private:
    vector<vector<GLint>> grid;
    // 2. 存储单个网格的顶点数据
    vector<GridVertex> vertices;
    // 3. 蛇身体
    vector<glm::vec2> snakeBody;
    // 4. 食物
    glm::vec2 food;
    GLuint VAO;
    GLuint VBO;
    Utils util;
    Shader shader;
    OpenGLErrors error;
    bool isInitialized = false;
    // 蛇移动的方向
};

#endif