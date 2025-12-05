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

GLuint vao_skybox;
GLuint vbo_skybox;
GLuint ebo_skybox;

GLuint vao_movewall;
GLuint vbo_movewall[2];
GLuint ebo_movewall;

GLuint vao_trap;
GLuint vbo_trap[2];
GLuint ebo_trap;


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
    float y = stickVertices[i][1];        // -0.5 ~ 0.5
    float t = (y + 0.5f);                 // 0.0 ~ 1.0 로 변환

    // 부드러운 갈색 그라데이션
    float r = 0.40f + 0.35f * t; // 0.40 ~ 0.75
    float g = 0.25f + 0.20f * t; // 0.25 ~ 0.45
    float b = 0.10f + 0.10f * t; // 0.10 ~ 0.20

    stickColors[i][0] = r;
    stickColors[i][1] = g;
    stickColors[i][2] = b;
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
        float y = pyramidVertices[i][1];      // 0.0 (바닥) ~ 1.0 (꼭대기)
        float t = y;                           // 그대로 0~1 사용

        // 부드러운 초록 그라데이션
        float r = 0.05f + 0.15f * t;  // 0.05 ~ 0.20
        float g = 0.30f + 0.55f * t;  // 0.30 ~ 0.80 (초록 강조)
        float b = 0.10f + 0.20f * t;  // 0.10 ~ 0.30

        pyramidColors[i][0] = r;
        pyramidColors[i][1] = g;
        pyramidColors[i][2] = b;
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
        float y = pyramidVertices[i][1]; // 0.0 ~ 1.0
        float t = y;

        // InitPyramidModel()과 동일한 초록 그라데이션 계산
        float baseR = 0.05f + 0.15f * t;
        float baseG = 0.30f + 0.55f * t;
        float baseB = 0.10f + 0.20f * t;

        // 그대로 복구하거나, 추가적으로 색조 조절도 가능
        pyramidColors[i][0] = baseR * r;
        pyramidColors[i][1] = baseG * g;
        pyramidColors[i][2] = baseB * b;
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

// ======================================================
// Skybox (배경 텍스처 전용)
// ======================================================

float skyVertices[] =
{
    // -------- Front face --------
    -1.0f, -1.0f,  1.0f,   0.0f, 0.0f,
     1.0f, -1.0f,  1.0f,   1.0f, 0.0f,
     1.0f,  1.0f,  1.0f,   1.0f, 1.0f,
    -1.0f,  1.0f,  1.0f,   0.0f, 1.0f,

    // -------- Back face --------
     1.0f, -1.0f, -1.0f,   0.0f, 0.0f,
    -1.0f, -1.0f, -1.0f,   1.0f, 0.0f,
    -1.0f,  1.0f, -1.0f,   1.0f, 1.0f,
     1.0f,  1.0f, -1.0f,   0.0f, 1.0f,

     // -------- Left face --------
     -1.0f, -1.0f, -1.0f,   0.0f, 0.0f,
     -1.0f, -1.0f,  1.0f,   1.0f, 0.0f,
     -1.0f,  1.0f,  1.0f,   1.0f, 1.0f,
     -1.0f,  1.0f, -1.0f,   0.0f, 1.0f,

     // -------- Right face --------
      1.0f, -1.0f,  1.0f,   0.0f, 0.0f,
      1.0f, -1.0f, -1.0f,   1.0f, 0.0f,
      1.0f,  1.0f, -1.0f,   1.0f, 1.0f,
      1.0f,  1.0f,  1.0f,   0.0f, 1.0f,

      // -------- Top face --------
      -1.0f,  1.0f,  1.0f,   0.0f, 0.0f,
       1.0f,  1.0f,  1.0f,   1.0f, 0.0f,
       1.0f,  1.0f, -1.0f,   1.0f, 1.0f,
      -1.0f,  1.0f, -1.0f,   0.0f, 1.0f,

      // -------- Bottom face --------
      -1.0f, -1.0f, -1.0f,   0.0f, 0.0f,
       1.0f, -1.0f, -1.0f,   1.0f, 0.0f,
       1.0f, -1.0f,  1.0f,   1.0f, 1.0f,
      -1.0f, -1.0f,  1.0f,   0.0f, 1.0f
};

unsigned int skyIndices[] =
{
    // Front
    0, 1, 2,   2, 3, 0,
    // Back
    4, 5, 6,   6, 7, 4,
    // Left
    8, 9, 10,  10, 11, 8,
    // Right
    12, 13, 14, 14, 15, 12,
    // Top
    16, 17, 18, 18, 19, 16,
    // Bottom
    20, 21, 22, 22, 23, 20
};

void InitSkyboxModel()
{
    glGenVertexArrays(1, &vao_skybox);
    glGenBuffers(1, &vbo_skybox);
    glGenBuffers(1, &ebo_skybox);

    glBindVertexArray(vao_skybox);

    glBindBuffer(GL_ARRAY_BUFFER, vbo_skybox);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyVertices), skyVertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_skybox);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(skyIndices), skyIndices, GL_STATIC_DRAW);

    // pos (location 0)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // uv (location 1)
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
}

// ======================================================
// MoveWall (움직이는 벽 전용)
// ======================================================

GLfloat movewallVertices[8][3] =
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

GLuint movewall_Indices[] =
{
    0,1,2, 2,3,0,
    4,5,6, 6,7,4,
    0,4,7, 7,3,0,
    1,5,6, 6,2,1,
    3,2,6, 6,7,3,
    0,1,5, 5,4,0
};

GLfloat movewallColors[8][3];

void InitMoveWallModel()
{
    glUseProgram(shaderProgramID);
    // 기본 색상
    for (int i = 0; i < 8; i++)
    {
        float y = movewallVertices[i][1];     // <- 올바른 버텍스 사용
        float t = (y + 0.5f);                 // 0 ~ 1

        float base = 0.25f + 0.25f * t;
        float highlight = 0.50f + 0.40f * t;
        float shadow = 0.15f * (1.0f - t);

        float r = base * 0.8f + highlight * 0.2f - shadow * 0.1f;
        float g = base * 0.85f + highlight * 0.15f;
        float b = base * 0.90f + highlight * 0.3f;

        r = glm::clamp(r, 0.0f, 1.0f);
        g = glm::clamp(g, 0.0f, 1.0f);
        b = glm::clamp(b, 0.0f, 1.0f);

        movewallColors[i][0] = r;   // <- 올바른 색상 배열에 저장
        movewallColors[i][1] = g;
        movewallColors[i][2] = b;
    }


    glGenVertexArrays(1, &vao_movewall);
    glBindVertexArray(vao_movewall);

    glGenBuffers(2, vbo_movewall);
    glGenBuffers(1, &ebo_movewall);

    // 위치 버퍼
    glBindBuffer(GL_ARRAY_BUFFER, vbo_movewall[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(movewallVertices), movewallVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    // 색상 버퍼
    glBindBuffer(GL_ARRAY_BUFFER, vbo_movewall[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(movewallColors), movewallColors, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(1);

    // 인덱스 버퍼
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_movewall);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(movewall_Indices), movewall_Indices, GL_STATIC_DRAW);
}

void UpdateMoveWallColor(float r, float g, float b)
{
    for (int i = 0; i < 8; i++)
    {
         movewallColors[i][0] = r;
         movewallColors[i][1] = g;
         movewallColors[i][2] = b;
    }

    glBindBuffer(GL_ARRAY_BUFFER, vbo_movewall[1]);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(movewallColors), movewallColors);
}

// ======================================================
// Trap (함정 전용)
// ======================================================

GLfloat trapVertices[4][3] =
{
    { -0.5f,  0.5f, -0.5f },   // top-left-front
    {  0.5f,  0.5f, -0.5f },   // top-right-front
    {  0.5f,  0.5f,  0.5f },   // top-right-back
    { -0.5f,  0.5f,  0.5f }    // top-left-back
};

GLuint trap_Indices[6] =
{
    0, 1, 2,
    2, 3, 0
};

GLfloat trapColors[4][3];

void InitTrapModel()
{
    glUseProgram(shaderProgramID);

    // 기본 색상 (밝은 회색)
    for (int i = 0; i < 4; i++)
    {
        float t = i / 3.0f;  // 0 → 1로 증가 (그라데이션 강제)

        // 기본 어두운 색 (딥 그레이)
        float baseR = 0.18f;
        float baseG = 0.18f;
        float baseB = 0.20f;

        // 그라데이션 밝기(약간만)
        float gradR = 0.10f;
        float gradG = 0.08f;
        float gradB = 0.06f;

        trapColors[i][0] = glm::clamp(baseR + t * gradR, 0.0f, 1.0f);
        trapColors[i][1] = glm::clamp(baseG + t * gradG, 0.0f, 1.0f);
        trapColors[i][2] = glm::clamp(baseB + t * gradB, 0.0f, 1.0f);
    }

    glGenVertexArrays(1, &vao_trap);
    glBindVertexArray(vao_trap);

    glGenBuffers(2, vbo_trap);
    glGenBuffers(1, &ebo_trap);

    // -------------------------
    // 위치 버퍼
    // -------------------------
    glBindBuffer(GL_ARRAY_BUFFER, vbo_trap[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(trapVertices), trapVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    // -------------------------
    // 색상 버퍼
    // -------------------------
    glBindBuffer(GL_ARRAY_BUFFER, vbo_trap[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(trapColors), trapColors, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(1);

    // -------------------------
    // 인덱스 버퍼 (윗면만)
    // -------------------------
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_trap);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(trap_Indices), trap_Indices, GL_STATIC_DRAW);
}

void UpdateTrapColor(float r, float g, float b)
{
    for (int i = 0; i < 4; i++)
    {
        trapColors[i][0] = r;
        trapColors[i][1] = g;
        trapColors[i][2] = b;
    }

    glBindBuffer(GL_ARRAY_BUFFER, vbo_trap[1]);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(trapColors), trapColors);
}