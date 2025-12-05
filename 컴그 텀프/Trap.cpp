#include "Trap.h"
#include "ShapeBuffer.h"
#include "Pyramid.h"
#include <gl/glm/gtc/matrix_transform.hpp>

// 생성자 -------------------------------
Trap::Trap(glm::vec3 p, glm::vec3 s)
{
    pos = p;
    size = s;

    spacing = 0.6f;

    int countX = (int)(size.x / spacing);
    int countZ = (int)(size.z / spacing);

    for (int ix = 0; ix < countX; ix++)
    {
        for (int iz = 0; iz < countZ; iz++)
        {
            float offsetX = -size.x * 0.5f + ix * spacing + spacing * 0.5f;
            float offsetZ = -size.z * 0.5f + iz * spacing + spacing * 0.5f;

            spikeOffsets.push_back(glm::vec3(offsetX, 0.0f, offsetZ));
        }
    }
}

//  외부에서 속도 변경 가능하도록
void Trap::SetSpeed(float s)
{
    speed = s;
}

// Update ----------------------------------------
void Trap::Update(float dt)
{
    spikeHeight += direction * speed * dt;

    if (spikeHeight >= 0.15f)
    {
        spikeHeight = 0.15f;
        direction = -1;
    }

    if (spikeHeight <= -2.0f)
    {
        spikeHeight = -2.0f;
        direction = 1;
    }
}

// Draw -------------------------------------------
void Trap::Draw(const glm::mat4& view, const glm::mat4& proj, GLuint mvpLoc)
{
    // ------------------------------------------
    // (1) 기존 피라미드 색 백업
    // ------------------------------------------
    GLfloat savedColors[5][3];
    memcpy(savedColors, pyramidColors, sizeof(pyramidColors));

    // ------------------------------------------
    // (2) 색상 변경 (빨강 또는 회색)
    // ------------------------------------------
    // 회색 예시 : UpdatePyramidColor(2.0f, 2.0f, 2.0f);
    // 빨강 예시 : UpdatePyramidColor(6.5f, 0.1f, 0.1f);

    // 원하는 색으로 변경
    UpdatePyramidColor(6.5f, 0.1f, 0.1f); //  빨간색
    // UpdatePyramidColor(2.0f, 2.0f, 2.0f); //  회색

    // ---------- 바닥 ----------
    {
        glm::mat4 m = glm::mat4(1.0f);
        m = glm::translate(m, pos);
        m = glm::scale(m, size);

        glm::mat4 mvp = proj * view * m;
        glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, &mvp[0][0]);

        glBindVertexArray(vao_trap);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    }

    // ---------- 스파이크 여러 개 ----------
    for (auto& offset : spikeOffsets)
    {
        glm::mat4 m = glm::mat4(1.0f);

        m = glm::translate(m, pos + glm::vec3(offset.x, spikeHeight, offset.z));
        m = glm::scale(m, spikeSize);

        glm::mat4 mvp2 = proj * view * m;
        glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, &mvp2[0][0]);

        glBindVertexArray(vao_pyramid);
        glDrawElements(GL_TRIANGLES, 18, GL_UNSIGNED_INT, 0);
    }

    // ------------------------------------------
    // (3) 색 복구
    // ------------------------------------------
    memcpy(pyramidColors, savedColors, sizeof(pyramidColors));
    glBindBuffer(GL_ARRAY_BUFFER, vbo_pyramid[1]);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(pyramidColors), pyramidColors);
}
