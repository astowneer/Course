#version 330 core

layout (location = 0) in vec3 aPos;


uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 camMatrix;

void main()
{
    gl_Position = camMatrix * (vec4(aPos.x,aPos.y,aPos.z-30, 1.0));
}