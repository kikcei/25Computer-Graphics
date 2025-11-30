#pragma once
#include <gl/glew.h>

char* filetobuf(const char* file);
void make_vertexShaders();
void make_fragmentShaders();
GLuint make_shaderProgram();

extern GLuint vertexShader;
extern GLuint fragmentShader;