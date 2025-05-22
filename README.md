<h2>运行截图：</h2>

<img src="https://github.com/InsideStart/The-Greedy-Snake/blob/main/images/%E8%B4%AA%E5%90%83%E8%9B%87.png" />

## 一、使用的库

- GLFW - 用于创建窗口和处理用户输入
- GLEW - 用于加载OpenGL函数
- GLU - 用与处理数学计算，尤其是矩阵和向量计算

## 二、项目内容

1. 使用GLFW进行窗口的创建和销毁、以及按键的检测
2. 使用VAO(Vertex Array Object,顶点数组对象)和VBO(Vertex Buffer Object,顶点缓冲对象)来保存数据
3. 使用GLSL代码和顶点着色器以及片段着色器进行渲染
4. 使用GLFW函数获取时间，进行帧率控制

## 三、项目不足

1. 项目打开时，没有设置程序直接获取焦点
2. 项目获取焦点时，输入法会改变为电脑的首选输入法
3. 窗口拉伸时，单个网格的拉伸比会改变，可能不再是一个正方形
4. 未设置游戏开始界面和游戏结束界面，程序打开则运行游戏，失败或关闭窗口则退出游戏
