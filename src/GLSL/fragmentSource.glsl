#version 450 core

in vec3 ourColor;

// the final color
out vec4 FragColor;

void main() {
    FragColor = vec4(ourColor,1.0);
}