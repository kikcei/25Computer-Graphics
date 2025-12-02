#include "ShapeBuffer.h"
#include "Shader.h"
#include <gl/glm/glm.hpp>

GLuint vao_stick;
GLuint vbo_stick[2];
GLuint ebo_stick;

GLuint vao_pyramid;
GLuint vbo_pyramid[2];
GLuint ebo_pyramid;

GLuint vao_floor;
GLuint vbo_floor;
GLuint ebo_floor;

// ======================================================
// Stick Geometry
// ======================================================
GLfloat stickVertices[8][3] =
{
    {-0.5f, -0.5f, -0.5f},
    { 0.5f, -0.5f, -0.5f},
    { 0.5f,  0.5f, -0.5f},
    {-0.5f,  0.5f, -0.5f},
    {-0.5f, -0.5f,  0.5f},
    { 0.5f, -0.5f,  0.5f},
    { 0.5f,  0.5f,  0.5f},
    {-0.5f,  0.5f,  0.5f}
};

GLuint stick_Indices[] =
{
    0,1,2, 2,3,0,
    4,5,6, 6,7,4,
    0,4,7, 7,3,0,
    1,5,6, 6,2,1,
    3,2,6, 6,7,3,
    0,1,5, 5,4,0
};

GLfloat stickColors[8][3];

void InitStickModel()
{
    glUseProgram(shaderProgramID);
    // 기본 색상
    for (int i = 0; i < 8; i++)
    {
        float t = (stickVertices[i][0] + 0.5f); // 0~1
        stickColors[i][0] = 0.4f * t + 0.2f;
        stickColors[i][1] = 0.7f;
        stickColors[i][2] = 1.0f;
    }

    glGenVertexArrays(1, &vao_stick);
    glBindVertexArray(vao_stick);

    glGenBuffers(2, vbo_stick);
    glGenBuffers(1, &ebo_stick);

    // 위치 버퍼
    glBindBuffer(GL_ARRAY_BUFFER, vbo_stick[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(stickVertices), stickVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    // 색상 버퍼
    glBindBuffer(GL_ARRAY_BUFFER, vbo_stick[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(stickColors), stickColors, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(1);

    // 인덱스 버퍼
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_stick);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(stick_Indices), stick_Indices, GL_STATIC_DRAW);
}

void UpdateStickColor(float r, float g, float b)
{
    for (int i = 0; i < 8; i++)
    {
        stickColors[i][0] = r;
        stickColors[i][1] = g;
        stickColors[i][2] = b;
    }

    glBindBuffer(GL_ARRAY_BUFFER, vbo_stick[1]);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(stickColors), stickColors);
}

// ======================================================
// Pyramid Geometry
// ======================================================

GLfloat pyramidVertices[][3] =
{
    {-0.5f, 0.0f, -0.5f},
    { 0.5f, 0.0f, -0.5f},
    { 0.5f, 0.0f,  0.5f},
    {-0.5f, 0.0f,  0.5f},
    { 0.0f, 1.0f,  0.0f}
};

GLuint pyramidIndices[] =
{
    0,1,4,
    1,2,4,
    2,3,4,
    3,0,4,
    0,1,2,
    0,2,3
};

GLfloat pyramidColors[5][3];

void InitPyramidModel()
{
    glUseProgram(shaderProgramID);
    for (int i = 0; i < 5; i++)
    {
        pyramidColors[i][0] = 0.55f;
        pyramidColors[i][1] = 0.75f;
        pyramidColors[i][2] = 0.55f;
    }

    glGenVertexArrays(1, &vao_pyramid);
    glBindVertexArray(vao_pyramid);

    glGenBuffers(2, vbo_pyramid);
    glGenBuffers(1, &ebo_pyramid);

    // 위치
    glBindBuffer(GL_ARRAY_BUFFER, vbo_pyramid[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(pyramidVertices), pyramidVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    // 색상
    glBindBuffer(GL_ARRAY_BUFFER, vbo_pyramid[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(pyramidColors), pyramidColors, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(1);

    // 인덱스 버퍼
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_pyramid);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(pyramidIndices), pyramidIndices, GL_STATIC_DRAW);
}

void UpdatePyramidColor(float r, float g, float b)
{
    for (int i = 0; i < 5; i++)
    {
        pyramidColors[i][0] = r;
        pyramidColors[i][1] = g;
        pyramidColors[i][2] = b;
    }

    glBindBuffer(GL_ARRAY_BUFFER, vbo_pyramid[1]);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(pyramidColors), pyramidColors);
}


// ======================================================
// Floor (윗면 텍스처 전용)
// ======================================================

// pos(x,y,z), uv(u,v)
GLfloat floorVertices[] =
{
    //   position           uv
    -0.5f, 0.0f, -0.5f,     0.0f, 0.0f,   // 0
     0.5f, 0.0f, -0.5f,     1.0f, 0.0f,   // 1
     0.5f, 0.0f,  0.5f,     1.0f, 1.0f,   // 2
    -0.5f, 0.0f,  0.5f,     0.0f, 1.0f    // 3
};

GLuint floorIndices[] =
{
    0, 1, 2,
    2, 3, 0
};

void InitFloorModel()
{
    glUseProgram(shaderProgramID);
    glGenVertexArrays(1, &vao_floor);
    glBindVertexArray(vao_floor);

    glGenBuffers(1, &vbo_floor);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_floor);
    glBufferData(GL_ARRAY_BUFFER, sizeof(floorVertices), floorVertices, GL_STATIC_DRAW);

    glGenBuffers(1, &ebo_floor);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_floor);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(floorIndices), floorIndices, GL_STATIC_DRAW);

    // position (location = 0)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // UV (location = 1)
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
}