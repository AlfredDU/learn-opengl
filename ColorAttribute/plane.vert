#version 330 core

// Input data
layout(location = 0) in vec3 position;
layout (location = 1) in vec3 color;

// ouput data
smooth out vec3 smoothColor;

void main(){

    gl_Position.xyz = position;
    gl_Position.w = 1.0;

    smoothColor = color;
}

