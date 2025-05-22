﻿#version 450 core

// vertex : position and color
layout(location = 0) in vec2 aPos;
layout(location = 1) in vec3 aColor;

uniform mat4 projection;

out vec3 ourColor;
void main() {
    gl_Position = projection * vec4(aPos,0.0,1.0);
    ourColor = aColor;
}