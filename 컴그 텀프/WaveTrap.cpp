#include "WaveTrap.h"
#include "ShapeBuffer.h"
#include "ShapeBuffer.h"

#include <gl/glm/gtc/matrix_transform.hpp>

// ---------------------------------------------------------
// 생성자: 타일 크기(x,z)를 기반으로 스파이크 자동 생성
// ---------------------------------------------------------
WaveTrap::WaveTrap(glm::vec3 p, glm::vec3 s)
{
    pos = p;
    size = s;

    int countX = (int)(size.x / spacing);
    int countZ = (int)(size.z / spacing);

    for (int ix = 0; ix < countX; ix++)
    {
        for (int iz = 0; iz < countZ; iz++)
        {
            float ox = -size.x * 0.5f + ix * spacing + spacing * 0.5f;
            float oz = -size.z * 0.5f + iz * spacing + spacing * 0.5f;

            // ==============================
           
            // ==============================
            float delay = (countZ - 1 - iz) * waveSpeed;

            spikes.push_back({ glm::vec3(ox, 0, oz), delay });
        }
    }
}

// ---------------------------------------------------------
// dt 기반 시간 증가
// ---------------------------------------------------------
void WaveTrap::Update(float dt)
{
    timer += dt;
}

// ---------------------------------------------------------
// Draw (바닥 + 여러 스파이크 웨이브)
// ---------------------------------------------------------
void WaveTrap::Draw(const glm::mat4& view, const glm::mat4& proj, GLuint mvpLoc)
{
    // (1) 기존 피라미드 색 백업
    GLfloat savedColors[5][3];
    memcpy(savedColors, pyramidColors, sizeof(pyramidColors));

    // (2) 원하는 색상 변화 적용
    // MoveWall과 같은 방식
    UpdatePyramidColor(5.0f, 0.1f, 0.1f);

    // ---------- 바닥 ----------
    glm::mat4 base = glm::mat4(1.0f);
    base = glm::translate(base, pos);
    base = glm::scale(base, size);

    glm::mat4 mvp = proj * view * base;
    glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, &mvp[0][0]);

    glBindVertexArray(vao_trap);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    // ---------- 스파이크 ----------
    for (auto& sp : spikes)
    {
        float localTime = timer - sp.delay;

        float minH = -2.0f;
        float maxH = 0.15f;

        float normalized = (localTime > 0 ? sin(localTime * speed) * 0.5f + 0.5f : 0);
        float height = minH + normalized * (maxH - minH);

        glm::mat4 sm = glm::mat4(1.0f);
        sm = glm::translate(sm, pos + glm::vec3(sp.offset.x, height, sp.offset.z));
        sm = glm::scale(sm, spikeSize);

        glm::mat4 mvp2 = proj * view * sm;
        glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, &mvp2[0][0]);

        glBindVertexArray(vao_pyramid);
        glDrawElements(GL_TRIANGLES, 18, GL_UNSIGNED_INT, 0);
    }

    // (3) 색 복구
    memcpy(pyramidColors, savedColors, sizeof(pyramidColors));
    glBindBuffer(GL_ARRAY_BUFFER, vbo_pyramid[1]);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(pyramidColors), pyramidColors);
}
