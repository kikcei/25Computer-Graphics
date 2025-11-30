#pragma once

#include <vector>
#include "Stick.h"
#include "Pyramid.h"

#include <gl/glew.h>
#include <gl/glm/glm.hpp>
#include <gl/glm/gtc/matrix_transform.hpp>


class BasicObstacle {
public:
    Stick body;
    std::vector<Pyramid> spikes;

    float angle = 0.0f;   // ← 회전값 추가!
    glm::vec3 rotationAxis = glm::vec3(0, 0, 1); // Z축 기준 회전

    BasicObstacle(glm::vec3 pos, float length, float spacing, float initialAngle = 0.0f);
    void Draw(const glm::mat4& view, const glm::mat4& proj, GLuint mvpLoc);
    void Update(float Time);
}; 
