#version 330 core

// Input data
layout(location = 0) in vec3 position;
layout (location = 1) in vec2 textureCoordinates;

// ouput data
out vec2 uv;

void main(){

    gl_Position.xyz = position;
    gl_Position.w = 1.0;

    uv = textureCoordinates;
}

