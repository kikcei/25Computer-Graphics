#pragma once

#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/glm/glm.hpp>
#include <gl/glm/ext.hpp>
#include <gl/glm/gtc/matrix_transform.hpp>

extern GLuint shaderProgramID;     // ´Ù¸¥ °´Ã¼µéÀÌ »ç¿ëÇÒ ½¦ÀÌ´õ

// ==============================
// Stick (Á÷»ç°¢Çü ¸ðµ¨)
// ==============================
extern GLuint vao_stick;
extern GLuint vbo_stick[2];
extern GLuint ebo_stick;

void InitStickModel();
void UpdateStickColor(float r, float g, float b);

// ==============================
// Pyramid (»ç°¢»Ô ¸ðµ¨)
// ==============================
extern GLuint vao_pyramid;
extern GLuint vbo_pyramid[2];
extern GLuint ebo_pyramid;

void InitPyramidModel();
void UpdatePyramidColor(float r, float g, float b);

// ==============================
// Floor (¹Ù´Ú ¸ðµ¨)
// ==============================
extern GLuint vao_floor;
extern GLuint vbo_floor;
extern GLuint ebo_floor;

void InitFloorModel();

// ==============================
// Skybox (¹è°æ ¸ðµ¨)
// ==============================

extern GLuint vao_skybox;
extern GLuint vbo_skybox;
extern GLuint ebo_skybox;

void InitSkyboxModel();
