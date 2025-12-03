#define _CRT_SECURE_NO_WARNINGS
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "Shader.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

GLuint vertexShader;
GLuint fragmentShader;

GLuint ShaderprogramID;


char* filetobuf(const char* file)
{
    FILE* fptr;
    long length;
    char* buf;

    fptr = fopen(file, "rb");
    if (!fptr)
        return NULL;
    fseek(fptr, 0, SEEK_END);
    length = ftell(fptr);
    buf = (char*)malloc(length + 1);
    fseek(fptr, 0, SEEK_SET);
    fread(buf, length, 1, fptr);
    fclose(fptr);

    buf[length] = 0;
    return buf;
}

void make_vertexShaders()
{
    GLchar* vertexSource = filetobuf("vertex.glsl");

    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexSource, NULL);
    glCompileShader(vertexShader);

    GLint result;
    GLchar errorLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &result);

    if (!result)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, errorLog);
        std::cerr << "ERROR: vertex shader 컴파일 실패\n" << errorLog << std::endl;
    }

    free(vertexSource);
}

void make_fragmentShaders()
{
    GLchar* fragmentSource = filetobuf("fragment.glsl");

    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
    glCompileShader(fragmentShader);

    GLint result;
    GLchar errorLog[512];
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &result);

    if (!result)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, errorLog);
        std::cerr << "ERROR: frag_shader 컴파일 실패\n" << errorLog << std::endl;
    }

    free(fragmentSource);
}

GLuint make_shaderProgram()
{
    make_vertexShaders();
    make_fragmentShaders();

    GLuint shaderID = glCreateProgram();
    glAttachShader(shaderID, vertexShader);
    glAttachShader(shaderID, fragmentShader);
    glLinkProgram(shaderID);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    GLint result;
    GLchar errorLog[512];
    glGetProgramiv(shaderID, GL_LINK_STATUS, &result);

    if (!result) {
        glGetProgramInfoLog(shaderID, 512, NULL, errorLog);
        std::cerr << "ERROR: shader program 연결 실패\n" << errorLog << std::endl;
    }

    return shaderID;
}

// ----------------------------------------
// 텍스처 전용 셰이더 로딩
// ----------------------------------------
GLuint loadTextureShader(const char* vertexPath, const char* fragmentPath)
{
    GLchar* vertexSource = filetobuf(vertexPath);
    GLchar* fragmentSource = filetobuf(fragmentPath);

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexSource, NULL);
    glCompileShader(vertexShader);

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
    glCompileShader(fragmentShader);

    GLuint programID = glCreateProgram();
    glAttachShader(programID, vertexShader);
    glAttachShader(programID, fragmentShader);
    glLinkProgram(programID);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    free(vertexSource);
    free(fragmentSource);

    // 텍스처 셰이더가 OpenGL의 현재 active shader로 남지 않도록 반드시 해제
    glUseProgram(0);

    return programID;
}

// ----------------------------------------
// 텍스처 로딩
// ----------------------------------------
GLuint LoadTexture(const char* filePath)
{
    stbi_set_flip_vertically_on_load(true);
    int width, height, channels;
    unsigned char* image = stbi_load(filePath, &width, &height, &channels, 4);

    if (!image) {
        printf("텍스처 로딩 실패: %s\n", filePath);
        return 0;
    }

    GLuint texID;
    glGenTextures(1, &texID);
    glBindTexture(GL_TEXTURE_2D, texID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(
        GL_TEXTURE_2D, 0, GL_RGBA,
        width, height, 0,
        GL_RGBA, GL_UNSIGNED_BYTE, image
    );

    stbi_image_free(image);

    return texID;
}

// ----------------------------------------
// 보트 로딩
// ----------------------------------------

static std::string ReadFile(const char* path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "Cannot open shader: " << path << std::endl;
        return "";
    }
    std::stringstream ss;
    ss << file.rdbuf();
    return ss.str();
}

static GLuint Compile(GLenum type, const char* src) {
    GLuint s = glCreateShader(type);
    glShaderSource(s, 1, &src, NULL);
    glCompileShader(s);

    GLint ok;
    glGetShaderiv(s, GL_COMPILE_STATUS, &ok);

    if (!ok) {
        char log[2048];
        glGetShaderInfoLog(s, 2048, NULL, log);
        std::cerr << "Shader compile error:\n" << log << std::endl;
    }
    return s;
}

GLuint LoadShader(const char* vpath, const char* fpath) {
    std::string v = ReadFile(vpath);
    std::string f = ReadFile(fpath);

    GLuint vs = Compile(GL_VERTEX_SHADER, v.c_str());
    GLuint fs = Compile(GL_FRAGMENT_SHADER, f.c_str());

    GLuint prog = glCreateProgram();
    glAttachShader(prog, vs);
    glAttachShader(prog, fs);

    glLinkProgram(prog);

    GLint ok;
    glGetProgramiv(prog, GL_LINK_STATUS, &ok);
    if (!ok) {
        char log[2048];
        glGetProgramInfoLog(prog, 2048, NULL, log);
        std::cerr << "Link error:\n" << log << std::endl;
    }

    glDeleteShader(vs);
    glDeleteShader(fs);

    return prog;
}
