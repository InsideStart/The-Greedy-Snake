#include "Game/Map.h"

#include <iostream>
Map::Map() : VAO(0),VBO(0){
    srand(time(NULL));
}

Map::~Map() {
    if(VAO != 0) glDeleteVertexArrays(1, &VAO);
    if(VBO != 0) glDeleteBuffers(1, &VBO);
}
void Map::framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    Map *map = static_cast<Map *>(glfwGetWindowUserPointer(window));
    if(map)
        map->reshape(width, height);
}

void Map::initData()
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    grid = vector<vector<GLint>>(GRID_WIDTH, vector<int>(GRID_HEIGHT, EMPTY));
    vertices.clear();
    GLint centerX = GRID_WIDTH / 2;
    GLint centerY = GRID_HEIGHT / 2;
    snakeBody = {glm::vec2(centerX, centerY), glm::vec2(centerX + 1, centerY), glm::vec2(centerX+2, centerY)};
    food = glm::vec2(centerX / 2, centerY / 2);
    
    updateVertexData();
}
void Map::setVertexAttributePointers() {
    glBindBuffer(GL_ARRAY_BUFFER,VBO);
    glVertexAttribPointer(0,                // 属性索引layout(location = 0)
                          2,                // 每个顶点的分量数vec2
                          GL_FLOAT,         // 数据类型
                          GL_FALSE,         // 是否归一化
                          sizeof(GridVertex), // 步长
                          (void*)offsetof(GridVertex,position)); // 数据起始偏移
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(GridVertex), (void *)offsetof(GridVertex, color));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
}
void Map::initMap() {

    error.checkOpenGLError("before initData in map");

    initData();
    error.checkOpenGLError("after initData in map");

    glBindVertexArray(VAO);
    setVertexAttributePointers();
    
    string vertexGLSL = util.getCurrentFile() + "/../../src/GLSL/vertexSource.glsl";
    string fragmentGLSL = util.getCurrentFile() + "/../../src/GLSL/fragmentSource.glsl";
    const char *vp, *fp;
    vp = vertexGLSL.c_str();
    fp = fragmentGLSL.c_str();

    shader.createShaderProgram(vp, fp);

    reshape(800,800);

    isInitialized = true;

    render();
    glBindVertexArray(0);
}

void Map::render() {
    if(!isInitialized){
        printf("Map dont init data.\n");
        return;
    }
    shader.use();
    bool res01 = error.checkOpenGLError("after use");
    if (res01)
    {
        exit(-1);
    }
    glBindVertexArray(VAO);
    GLsizei count = static_cast<GLsizei>(vertices.size());
    if(count % 3 != 0) {
        std::cerr << "vertices cant use." << std::endl;
        exit(EXIT_FAILURE);
    }
    error.checkOpenGLError("before glDrawArrays");
    glDrawArrays(GL_TRIANGLES, 0, count);
    error.checkOpenGLError("after glDrawArrays");

    glBindVertexArray(0);
}

void Map::updateVertexData() {
    vertices.clear();
    grid = vector<vector<GLint>>(GRID_WIDTH, vector<int>(GRID_HEIGHT, EMPTY));
    for (const auto &segment : snakeBody)
    {
        // std::cout << "(" << segment.x << " , " << segment.y << ")" << std::endl;
        GLint x = static_cast<GLint>(segment.x);
        GLint y = static_cast<GLint>(segment.y);
        grid[x][y] = SNAKE;
    }
    GLint food_x = static_cast<GLint>(food.x);
    GLint food_y = static_cast<GLint>(food.y);
    grid[food_x][food_y] = FOOD;
    for (int y = 0; y < GRID_HEIGHT; ++y)
    {
        for (int x = 0; x < GRID_WIDTH; ++x)
        {
            glm::vec3 grid_color = glm::vec3(0.0f, 0.0f, 0.0f);
            if (grid[x][y] == SNAKE)
            {
                grid_color = SNAKE_COLOR;
            }
            else if (grid[x][y] == FOOD)
            {
                grid_color = FOOD_COLOR;
            }
            else
            {
                grid_color = glm::vec3(1.0f, 1.0f, 1.0f); // 不可见
            }
            vertices.push_back({glm::vec2(x * CELL_SIZE, y * CELL_SIZE), grid_color});
            vertices.push_back({glm::vec2(x * CELL_SIZE, (y + 1) * CELL_SIZE), grid_color});
            vertices.push_back({glm::vec2((x + 1) * CELL_SIZE, (y + 1) * CELL_SIZE), grid_color});
            vertices.push_back({glm::vec2(x * CELL_SIZE, y * CELL_SIZE), grid_color});
            vertices.push_back({glm::vec2((x + 1) * CELL_SIZE, y * CELL_SIZE), grid_color});
            vertices.push_back({glm::vec2((x + 1) * CELL_SIZE, (y + 1) * CELL_SIZE), grid_color});
        }
    }
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GridVertex),
                 vertices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Map::reshape(int width, int height)
{
    glViewport(0, 0, width, height); // 更新视口

    glm::mat4 projection = glm::ortho(
        0.0f,
        static_cast<GLfloat>(GRID_WIDTH * CELL_SIZE),
        0.0f,
        static_cast<GLfloat>(GRID_HEIGHT * CELL_SIZE));

    shader.use();
    shader.setMat4("projection", projection);
}

void Map::keyboard_input(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    if(action == GLFW_PRESS || action == GLFW_REPEAT) {
        switch (key)
        {
        case GLFW_KEY_W:
            /* code */
            if(direction != DOWN)
                direction = UP;
            break;
        case GLFW_KEY_A:
            if(direction != RIGHT)
                direction = LEFT;
            break;
        case GLFW_KEY_S:
            if (direction != UP)
                direction = DOWN;
            break;
        case GLFW_KEY_D:
            if (direction != LEFT)
                direction = RIGHT;
            break;
        default:
            break;
        }
    }
}

void Map::updateSnake() {
    glm::vec2 newVertex;
    GLfloat head_x = static_cast<GLfloat>(snakeBody[0].x);
    GLfloat head_y = static_cast<GLfloat>(snakeBody[0].y);
    switch (direction)
    {
    case LEFT:
        /* code */
        newVertex = glm::vec2(head_x - 1, head_y);
        break;
    case UP:
        /* code */
        newVertex = glm::vec2(head_x, head_y + 1);
        break;
    case RIGHT:
        /* code */
        newVertex = glm::vec2(head_x + 1, head_y);
        break;
    case DOWN:
        /* code */
        newVertex = glm::vec2(head_x, head_y - 1);
        break;
    }
    if(newVertex.x < 0 || newVertex.x >= GRID_WIDTH || newVertex.y < 0 || newVertex.y >= GRID_HEIGHT) {
        std::cout << "Game Over : Hit the wall!" << std::endl;
        exit(EXIT_FAILURE);
    }
    for(const auto& segment:snakeBody) {
        if(newVertex == segment) {
            std::cout << "Game Over : Ate yourself!" << std::endl;
            exit(EXIT_FAILURE);
        }
    }
    snakeBody.insert(snakeBody.begin(), newVertex);
    // 检测食物
    if(newVertex != food) {
        snakeBody.pop_back();
    }else {
        // 1. 生成新的食物
        int num = rand() % GRID_WIDTH;
        glm::vec2 newFood = glm::vec2(num,num);
        while(1) {
            bool isCon = false;
            for(const auto& segment:snakeBody) {
                if(newFood == segment) {
                    num = rand() % GRID_WIDTH;
                    std::cout << "rand num : " << num << std::endl;
                    newFood = glm::vec2(num, num);
                    isCon = true;
                }
            }
            if(!isCon)
                break;
        }
        // 2. 赋予新的食物坐标
        food = newFood;
    }

    error.checkOpenGLError("before updateVertexData error!");
    updateVertexData();
    error.checkOpenGLError("after updateVertexData error!");
}