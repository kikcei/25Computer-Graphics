#version 330 core
layout (location = 0) in vec3 aPos;

out vec3 TexDir;
uniform mat4 vp;

void main()
{
    TexDir = aPos;
    gl_Position = vp * vec4(aPos, 1.0);
}