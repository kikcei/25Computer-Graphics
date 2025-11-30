#version 330 core
layout (location = 0) in vec3 in_Position;         // VBO 첫 번째 속성 위치
layout (location = 1) in vec3 in_Color;            // VBO 두 번째 속성 색상
out vec3 out_Color;
uniform mat4 mvp;
void main(void)
{
    gl_Position = mvp * vec4(in_Position, 1.0);
    out_Color = in_Color;
}