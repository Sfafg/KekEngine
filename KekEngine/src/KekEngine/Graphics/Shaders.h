#pragma once
const char *vertexShaderSource =
    R"(
#version 330 core 
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec3 aCol;
out vec3 col;
uniform vec2 offset;
void main()
{
    gl_Position = vec4(aPos.x + offset.x, aPos.y - offset.y, 0.0, 1.0);
    col = aCol;
})";

const char *fragmentShaderSource =
    R"(
#version 330 core
out vec4 FragColor;
in vec3 col;
void main()
{
   FragColor = vec4(col.x, col.y, col.z, 1.0f);
})";