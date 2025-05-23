## 一、创建显示窗口

1. 初始化GLFW
2. 进入主循环 - 不断刷新窗口
3. 清除窗口

### 1. 初始化GLFW

```C++
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

glfwSwapInterval(0);
```

#### 1.1 设置OpenGL版本 - 需要查看自身电脑支持的OpenGL版本并设置

```C++
// 代表使用的OpenGL版本是4.5版本
glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,4);
glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,5);
```

#### 1.2 强制使用核心模式

```C++
glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
```

- **禁用遗留功能**：仅支持OpenGL3.2及更高版本的现代特性，移除旧版的立即渲染模式（如`glBegin/glEnd`）等过时API
- **强制现代化变成**：必须使用基于着色器的渲染管线
- **提升兼容性**：确保代码遵循现代图形标准
- **需要和版本号设置配合使用才可以生效**

 **跨平台注意事项**

- macOS 特殊要求

  在 macOS 系统中，需额外添加前向兼容设置才能启用核心模式

  ```cpp
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  ```

- **硬件兼容性验证**：若显卡不支持指定版本的核心模式，窗口创建会失败（可通过 `glfwCreateWindow` 返回值判断）

#### 1.3 创建窗口并显示

```C++
GLFWwindow *window = glfwCreateWindow(800, 800, "opengl-snake", NULL, NULL);
if(!window) {
    std::cerr << "Create window failed." << std::endl;
    glfwTerminate();
    exit(EXIT_FAILURE);
}
glfwMakeContextCurrent(window);
```

- 需要设置当前上下文为创建的窗口。在创建窗口之后，在任何OpenGL操作之前调用

  可以使用`glfwGetCurrentContext()`来查询当前线程的上下文

#### 1.4 设置垂直同步

> [!Note]
>
> 在设置上下文生效后调用
>
> 1. 画面撕裂：
>
>    - 如果显卡渲染一帧的时间 **短于** 显示器的刷新周期（例如显卡每秒渲染 120 帧，显示器每秒刷新 60 次），则可能出现 **画面撕裂**：
>      显示器在刷新时，显卡已经渲染了下一帧的部分内容，导致同一帧的不同部分被显示在不同位置。
>    - 如果显卡渲染速度 **远快于** 显示器刷新频率，帧率波动可能导致卡顿。
>
> 2. 垂直同步：
>
>    - 基本原理：
>
>      开启垂直同步后，显卡会 **等待显示器完成当前帧的刷新** 后，才提交下一帧的数据到屏幕。
>
>      **同步点**：显卡仅在显示器完成一次垂直扫描（即一帧绘制完成）时发送新帧。
>
>    - **效果**
>
>      - **优点**：彻底消除画面撕裂。
>      - 缺点：
>        - 帧率被限制为显示器的刷新频率（如 60Hz → 最大 60 FPS）。
>        - 可能引入 **输入延迟**（Input Lag），因为显卡需等待显示器刷新完成。

```C++
glfwSwapInterval(0); // 禁用垂直同步
```

当`glfwSwapInterval`的值为1是表示启用垂直同步

### 2. 进入主循环

> [!Note]
>
> 双重缓冲区：**双缓冲（double buffering）** 是一种避免画面撕裂和闪烁的绘图机制：
>
> - 有两个帧缓冲区：一个“前缓冲区”（显示在屏幕上），一个“后缓冲区”（你在绘制的）。
> - 每一帧你在后缓冲区里画，**画完后通过“交换”操作**（swap）把它显示出来。
>
> **交换前后缓冲区，使得绘制的图像显示到屏幕上 - `glfSwapBuffers(GLFWwindow*)`
>
> - 作用：让你刚刚在后缓冲区画好的画面显示到屏幕上
> - 调用时机，通常每帧绘制结束后调用一次

```C++
while(!glfwWindowShouldClose(window)) {
    // 设置glfwPollEvents可以相应在窗口上的各种事件 - 如点击等
    glfwPollEvents();
    glfwSwapBuffers(window);
}
```

### 3. 清除窗口

```C++
glfwTerminate();
```

用于**释放 GLFW 占用的所有系统资源并终止 GLFW 库的运行**

## 二、创建地图

### 1. 创建相关数据

```C++
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
```

> [!warning]
>
> 使用pack和push很有必要，可以使结构体内部强制对齐
>
> GPU通常以固定大小的块（如4字节、8字节）读取顶点数据，若没有对齐存储可能导致：
>
> 1. 性能下降 - GPU需要多次读取内存合并数据
> 2. 数据错误 - 部分数据被截断或错位（如vec3被拆分为两个4字节块）
>
> OpenGL默认的对齐规则：
>
> 1. 每个顶点属性的起始地址必须是其类型大小的整数倍
> 2. 未满足对齐要求，OpenGL会隐式的插入填充字节，但开发者需要显式的管理内存布局

### 2. GLSL代码

#### 2.1 顶点着色器代码

```glsl
#version 450 core

// vertex : position and color
layout(location = 0) in vec2 aPos;
layout(location = 1) in vec3 aColor;

// 应用图形变化 - 注意：在GLSL中不要使用中文注释
uniform mat4 projection;

out vec3 ourColor;
void main() {
    gl_Position = projection * vec4(aPos,0.0,1.0);
    ourColor = aColor;
}
```

- `uniform`  - 设置外部绑定，外界可以使用

  ```C++
  // 绑定数据
  GLint loc = glGetUniformLocation(resProgram, uniformMat4.c_str());
  if(loc == -1) {
      std::cerr << "Uniform " << uniformMat4 << " not found!" << std::endl;
      return;
  }
  glUniformMatrix4fv(loc, 1, GL_FALSE, &mat4[0][0]);
  ```

#### 2.2 片段着色器代码

```glsl
#version 450 core

in vec3 ourColor;

// the final color
out vec4 FragColor;

void main() {
    FragColor = vec4(ourColor,1.0);
}
```

### 3. VAO和VBO

> [!Note]
>
> VAO - Vertex Array Object 顶点数组对象
>
> VBO - Vertex Buffer Object 顶点缓冲对象
>
> 目的是绑定数据并将数据配合GLSL一起被着色器程序生成
>
> 一定要先生成绑定VAO在绑定VBO，在不使用的时候可以解绑以免对数据误操作
>
> warning ： 不可以重复绑定

#### 3.1 创建VAO和VBO

```C++
GLuint VAO,VBO;
glGenVertexArrays(1, &VAO);
glGenBuffers(1, &VBO);
```

#### 3.2 绑定VAO和VBO

```C++

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
glBindBuffer(GL_ARRAY_BUFFER, 0); // 解绑VBO


```

1. 先绑定VAO，再绑定VBO
2. 先解绑VBO，再解绑VAO

#### 3.3 数据和VBO绑定

```C++
glBindVertexArray(VAO);
glBindBuffer(GL_ARRAY_BUFFER, VBO);
glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GridVertex),
             vertices.data(), GL_STATIC_DRAW);
glBindBuffer(GL_ARRAY_BUFFER, 0);
glBindVertexArray(0);
```

#### 3.4 删除VAO和VBO

```C++
if(VAO != 0) glDeleteVertexArrays(1, &VAO);
if(VBO != 0) glDeleteBuffers(1, &VBO);
```

### 4. Shader着色器

#### 4.1 创建着色器程序

```C++
// vShaderSource - const char* - 顶点着色器的字符串
// fShaderSource - const char* - 片段着色器的字符串
glShaderSource(vShader, 1, &vShaderSource, NULL);
glShaderSource(fShader, 1, &fShaderSource, NULL);

glCompileShader(vShader);
glCompileShader(fShader);

resProgram = glCreateProgram();
glAttachShader(resProgram, vShader);
glAttachShader(resProgram, fShader);

glLinkProgram(resProgram);
```

获取GLSL代码的字符串

```C++
string Utils::readShaderSource(const char* path) {
    string content;
    std::ifstream fileStream(path,std::ios::in);
    string line = "";
    while(!fileStream.eof()) {
        std::getline(fileStream, line);
        content.append(line + '\n');
    }
    fileStream.close();
    return content;
}
```

#### 4.2 使用着色器程序

```C++
void Shader::use() {
    glUseProgram(resProgram);
}
```

在生成渲染时，一定要先调用use函数来使用着色器。

### 5. 绘制地图

```C++
glDrawArrays(GL_TRIANGLES, 0, count);
```

**基础图元：**

- `GL_TRIANGLES` - 绘制三角形 ， 则count表示顶点数组的size，必须为3的整数倍
- `GL_TRIANGLES_STRIP` - 绘制连续三角形
- `GL_TRIANGLE_FAN` - 绘制扇形三角形
- `GL_POINTS` - 绘制点
- `GL_LINES` - 绘制独立线段
- `GL_LINE_STRIP` - 绘制连续线段
- `GL_LINE_LOOP` - 绘制闭合线段

## 三、其他工具

### 1. 按键检查

```C++
// 添加按键检测 - 传递函数指针
glfwSetKeyCallback(window, Map::keyboard_input);
```

`Map::keyboard_input`

```C++
void Map::keyboard_input(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    // direction是设置的一个变量，DOWN...是枚举值
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
```

### 2. 添加类型指针进入OpenGL

```C++
Map map;
glfwSetWindowUserPointer(window, &map);

// 在其他地方使用：
Map *map = static_cast<Map *>(glfwGetWindowUserPointer(window));
```

设置窗口变化函数：

```C++
glfwSetFramebufferSizeCallback(window, Map::framebuffer_size_callback);
```

```C++
void Map::framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    Map *map = static_cast<Map *>(glfwGetWindowUserPointer(window));
    if(map)
        map->reshape(width, height);
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

```

