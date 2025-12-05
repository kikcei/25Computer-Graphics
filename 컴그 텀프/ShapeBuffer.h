#pragma once

#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/glm/glm.hpp>
#include <gl/glm/ext.hpp>
#include <gl/glm/gtc/matrix_transform.hpp>

extern GLuint shaderProgramID;     // 다른 객체들이 사용할 쉐이더

// ==============================
// Stick (직사각형 모델)
// ==============================
extern GLuint vao_stick;
extern GLuint vbo_stick[2];
extern GLuint ebo_stick;

void InitStickModel();
void UpdateStickColor(float r, float g, float b);

// ==============================
// Pyramid (사각뿔 모델)
// ==============================
extern GLuint vao_pyramid;
extern GLuint vbo_pyramid[2];
extern GLuint ebo_pyramid;

void InitPyramidModel();
void UpdatePyramidColor(float r, float g, float b);

// ==============================
// Floor (바닥 모델)
// ==============================
extern GLuint vao_floor;
extern GLuint vbo_floor;
extern GLuint ebo_floor;

void InitFloorModel();

// ==============================
// Skybox (배경 모델)
// ==============================
extern GLuint vao_skybox;
extern GLuint vbo_skybox;
extern GLuint ebo_skybox;

void InitSkyboxModel();

// ==============================
// MoveWall (움직이는 벽 모델)
// ==============================
extern GLuint vao_movewall;
extern GLuint vbo_movewall[2];
extern GLuint ebo_movewallk;

void InitMoveWallModel();
void UpdateMoveWallColor(float r, float g, float b);

// ==============================
// Trap (함정 모델)
// ==============================
extern GLuint vao_trap;
extern GLuint vbo_trap[2];
extern GLuint ebo_trap;

void InitTrapModel();
void UpdateTrapColor(float r, float g, float b);

//------------------------------------------------------------------------------
// 

// Stick 관련
extern GLfloat stickVertices[8][3];
extern GLuint  stick_Indices[36];
extern GLfloat stickColors[8][3];

// Pyramid 관련
extern GLfloat pyramidVertices[5][3];
extern GLuint  pyramidIndices[18];
extern GLfloat pyramidColors[5][3];

// Skybox 관련 (원하면)
extern float skyVertices[/* 정확한 크기 */];
extern unsigned int skyIndices[/* 정확한 크기 */];

// Floor 관련
extern GLfloat floorVertices[20];
extern GLuint  floorIndices[6];

// MoveWall 관련
extern GLfloat movewallVertices[8][3];
extern GLuint  movewall_Indices[36];
extern GLfloat movewallColors[8][3];

// Trap 관련
extern GLfloat trapVertices[4][3];
extern GLuint  trap_Indices[6];
extern GLfloat trapColors[4][3];
