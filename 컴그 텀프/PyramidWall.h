#pragma once

#include <vector>
#include "Stick.h"
#include "Pyramid.h"

#include <gl/glew.h>
#include <gl/glm/glm.hpp>
#include <gl/glm/gtc/matrix_transform.hpp>


class PyramidWall {
public:
    Stick body;
    std::vector<Pyramid> spikes;

    float basic_angle = 0.0f; // <- 도형 기본 회전값 추가
    float angle = 0.0f;   //  <- 회전값 추가!

    glm::vec3 basic_Axis = glm::vec3(0, 0, 0); // Z축 기준 회전
    glm::vec3 rotationAxis = glm::vec3(0, 0, 1); // Z축 기준 회전

    PyramidWall(glm::vec3 pos, float length, float spacing, glm::vec3 basicaxis, glm::vec3 axis,float basicangle = 0.0f, float initialAngle = 0.0f, float stickY = 0.2f, float stickZ = 0.2f, float PyramidHeight = 0.2f);
    void Draw(const glm::mat4& view, const glm::mat4& proj, GLuint mvpLoc);
    void Update(float Time);
}; 
