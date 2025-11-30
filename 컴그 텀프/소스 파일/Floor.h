#pragma once
#include <vector>
#include <gl/glew.h>
#include <gl/glm/glm.hpp>
#include <gl/glm/gtc/matrix_transform.hpp>

class Floor {
public:
    glm::vec3 pos;     // 위치
    glm::vec3 size;    // 스케일

    Floor(glm::vec3 p, glm::vec3 s);
    void Draw(const glm::mat4& view, const glm::mat4& proj, GLuint mvpLoc);
}; 
