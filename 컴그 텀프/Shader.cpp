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

GLuint vertexShader;
GLuint fragmentShader;

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

GLuint loadTextureShader(const char* vertexPath, const char* fragmentPath)
{
    // ------------------------------
    // 1. 파일 읽기
    // ------------------------------
    GLchar* vertexSource = filetobuf(vertexPath);
    GLchar* fragmentSource = filetobuf(fragmentPath);

    // ------------------------------
    // 2. 셰이더 생성
    // ------------------------------
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexSource, NULL);
    glCompileShader(vertexShader);

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
    glCompileShader(fragmentShader);

    // ------------------------------
    // 3. 프로그램 링크
    // ------------------------------
    GLuint programID = glCreateProgram();
    glAttachShader(programID, vertexShader);
    glAttachShader(programID, fragmentShader);
    glLinkProgram(programID);

    // ------------------------------
    // 4. 메모리 정리
    // ------------------------------
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    free(vertexSource);
    free(fragmentSource);

    return programID;
}

GLuint LoadTexture(const char* filePath)
{
    int width, height, channels;
    unsigned char* image = stbi_load(filePath, &width, &height, &channels, 4);

    if (!image) {
        printf("텍스처 로딩 실패: %s\n", filePath);
        return 0;
    }

    GLuint texID;
    glGenTextures(1, &texID);
    glBindTexture(GL_TEXTURE_2D, texID);

    // Wrap 옵션
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // Filtering 옵션
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // 실제 텍스처 데이터 업로드
    glTexImage2D(
        GL_TEXTURE_2D, 0, GL_RGBA,
        width, height, 0,
        GL_RGBA, GL_UNSIGNED_BYTE, image
    );

    // 메모리 해제
    stbi_image_free(image);

    return texID;
}

GLuint LoadShader(const char* vpath, const char* fpath)
{

	auto read = [&](const char* p) {
		std::ifstream fs(p);
		std::stringstream ss;
		ss << fs.rdbuf( );
		return ss.str( );
		};

	std::string vs = read(vpath);
	std::string fs = read(fpath);

	GLuint VS = glCreateShader(GL_VERTEX_SHADER);
	const char* vsrc = vs.c_str( );
	glShaderSource(VS, 1, &vsrc, NULL);
	glCompileShader(VS);

	GLuint FS = glCreateShader(GL_FRAGMENT_SHADER);
	const char* fsrc = fs.c_str( );
	glShaderSource(FS, 1, &fsrc, NULL);
	glCompileShader(FS);

	GLuint prog = glCreateProgram( );
	glAttachShader(prog, VS);
	glAttachShader(prog, FS);
	glLinkProgram(prog);

	glDeleteShader(VS);
	glDeleteShader(FS);
	auto check = [](GLuint sh, const char* name) {
		GLint success;
		glGetShaderiv(sh, GL_COMPILE_STATUS, &success);
		if (!success) {
			char log[ 2048 ];
			glGetShaderInfoLog(sh, 2048, NULL, log);
			printf("Shader Compile Error (%s): %s\n", name, log);
		}
		};

	check(VS, "VERTEX");
	check(FS, "FRAGMENT");

	GLint linkSuccess;
	glGetProgramiv(prog, GL_LINK_STATUS, &linkSuccess);
	if (!linkSuccess) {
		char log[ 2048 ];
		glGetProgramInfoLog(prog, 2048, NULL, log);
		printf("Program Link Error: %s\n", log);
	}


	return prog;
}
