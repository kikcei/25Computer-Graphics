#pragma once
#include <gl/glew.h>
#include <gl/glm/glm.hpp>

class Trap {
public:
    glm::vec3 pos;
    glm::vec3 size;

    float spikeHeight = 0.0f;   // 0 = 바닥, 1 = 최대 돌출

    Trap(glm::vec3 p, glm::vec3 s);
    void Update(const glm::vec3& boatPos);
    void Draw(const glm::mat4& view, const glm::mat4& proj, GLuint mvpLoc);
};
