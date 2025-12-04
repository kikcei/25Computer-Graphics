#pragma once
#include <vector>
#include <gl/glew.h>
#include <gl/glm/glm.hpp>
#include <gl/glm/gtc/matrix_transform.hpp>

#include "Pyramid.h"

class MoveWall {
public:

    glm::vec3 pos;     // 벽 위치
    glm::vec3 size;    // 벽 크기(스케일)

    float spikeSize;   // 피라미드(가시) 크기
    float spacing;     // 피라미드 간격
    float rotateAngle; // 벽 회전값

    // 벽에 붙어 있는 피라미드들
    std::vector<Pyramid> spikes;

    //  MoveWall.cpp에서 사용한 확장된 생성자
    MoveWall(glm::vec3 p,
        glm::vec3 wallSize,
        float spikeS,
        float sp,
        float rot = 0.0f);

    void Draw(const glm::mat4& view, const glm::mat4& proj, GLuint mvpLoc);
};
