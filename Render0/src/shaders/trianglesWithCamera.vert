#version 330 core

layout( location = 0 ) in vec4 vPosition;
layout( location = 1 ) in vec4 vColor;

uniform mat4 vProjection;

out vec4 fColor;

void main()
{
    gl_Position = vProjection * vPosition;
    fColor = vColor;
}
