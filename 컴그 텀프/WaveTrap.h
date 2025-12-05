#pragma once
#include <vector>
#include <gl/glew.h>
#include <gl/glm/glm.hpp>

struct WaveSpike {
    glm::vec3 offset;   // 스파이크가 타일 중심에서 얼마나 떨어져 있는지
    float delay;        // 파도 지연 시간
};

class WaveTrap {
public:
    glm::vec3 pos;      // 트랩 중심 위치
    glm::vec3 size;     // 바닥 크기 (x, y, z)

    float spacing = 0.6f;                      // 스파이크 간격
    glm::vec3 spikeSize = glm::vec3(0.2f, 0.35f, 0.2f);  // 스파이크 크기

    float timer = 0.0f;       // 전체 경과 시간
    float speed = 2.0f;       // 파형 속도
    float waveSpeed = 0.25f;  // 파도 퍼지는 속도

    std::vector<WaveSpike> spikes;

    WaveTrap(glm::vec3 p, glm::vec3 s);

    void Update(float dt);
    void Draw(const glm::mat4& view, const glm::mat4& proj, GLuint mvpLoc);
};