#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aUV;

out vec2 uv;

uniform mat4 mvp;
uniform vec2 uvOffset;   // 애니메이션

void main()
{
    gl_Position = mvp * vec4(aPos, 1.0);
   uv = aUV + uvOffset;
}