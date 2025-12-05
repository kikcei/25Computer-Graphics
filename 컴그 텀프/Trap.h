#pragma once
#include <vector>
#include <gl/glew.h>
#include <gl/glm/glm.hpp>

class Trap {
public:
    glm::vec3 pos;
    glm::vec3 size;

    float spacing;
    float spikeHeight = -2.0f;
    int direction = 1;

    float speed = 0.5f;  //  외부에서 조절 가능하도록 기본값 설정

    glm::vec3 spikeSize = glm::vec3(0.2f, 0.4f, 0.2f);
    std::vector<glm::vec3> spikeOffsets;

    Trap(glm::vec3 p, glm::vec3 s);

    void SetSpeed(float s);   //  속도 변경 함수 추가
    void Update(float dt);
    void Draw(const glm::mat4& view, const glm::mat4& proj, GLuint mvpLoc);
};
