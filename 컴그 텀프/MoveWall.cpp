#include "MoveWall.h"
#include "Pyramid.h"
#include "ShapeBuffer.h"
#include "Shader.h"

MoveWall::MoveWall(glm::vec3 p,
    glm::vec3 wallSize,
    float spikeS,
    float sp,
    float rot)
{
    pos = p;
    size = wallSize;
    spikeSize = spikeS;
    spacing = sp;
    rotateAngle = rot;

    // 스파이크 개수 자동 계산
    int count = wallSize.x / spacing;

    for (int i = 0; i < count; i++)
    {
        float offset = (i - count / 2.0f) * spacing;

        // 상단 스파이크
        Pyramid top(
            glm::vec3(pos.x + offset + spacing/2, pos.y + wallSize.y * 0.5f, pos.z),
            glm::vec3(spikeSize, spikeSize, spikeSize)
        );
        top.rotation = glm::vec3(0, 0, 0);
        spikes.push_back(top);

        // 하단 스파이크
        Pyramid bottom(
            glm::vec3(pos.x + offset + spacing / 2, pos.y - wallSize.y * 0.5f, pos.z),
            glm::vec3(spikeSize, spikeSize, spikeSize)
        );
        bottom.rotation = glm::vec3(180.0f, 0, 0);
        spikes.push_back(bottom);

        // 오른쪽 스파이크
        Pyramid right(
            glm::vec3(pos.x + wallSize.x * 0.5f,   pos.y + offset + spacing / 2, pos.z),
            glm::vec3(spikeSize)
        );
        right.rotation = glm::vec3(0, 0, -90.0f);
        spikes.push_back(right);

        // 왼쪽 스파이크
        Pyramid left(
            glm::vec3(pos.x - wallSize.x * 0.5f,   pos.y + offset + spacing / 2, pos.z),
            glm::vec3(spikeSize)
        );
        left.rotation = glm::vec3(0, 0, 90.0f);
        spikes.push_back(left);
    }
}

void MoveWall::Draw(const glm::mat4& view, const glm::mat4& proj, GLuint mvpLoc)
{
    // ---------------------------
    // (1) 현재 Pyramid 색상 백업
    // ---------------------------
    GLfloat savedColors[5][3];
    memcpy(savedColors, pyramidColors, sizeof(pyramidColors));

    // ---------------------------
    // (2) MoveWall 스파이크 색 설정
    // ---------------------------
    UpdatePyramidColor(0.3f, 0.5f, 6.0f);

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, pos);
    model = glm::rotate(model, glm::radians(rotateAngle), glm::vec3(0, 1, 0));
    model = glm::scale(model, size);

    glm::mat4 mvp = proj * view * model;
    glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, &mvp[0][0]);

    glBindVertexArray(vao_movewall);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

    // 스파이크도 그리기
    for (auto& sp : spikes)
    {
        glm::mat4 sm = glm::mat4(1.0f);
        sm = glm::translate(sm, sp.pos);
        sm = glm::rotate(sm, glm::radians(sp.rotation.x), glm::vec3(1, 0, 0));
        sm = glm::rotate(sm, glm::radians(sp.rotation.y), glm::vec3(0, 1, 0));
        sm = glm::rotate(sm, glm::radians(sp.rotation.z), glm::vec3(0, 0, 1));
        sm = glm::scale(sm, sp.size);

        glm::mat4 mvp2 = proj * view * sm;
        glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, &mvp2[0][0]);

        glBindVertexArray(vao_pyramid);
        glDrawElements(GL_TRIANGLES, 18, GL_UNSIGNED_INT, 0);
    }
    // ---------------------------
    // (3) Pyramid 색상 복구
    // ---------------------------
    memcpy(pyramidColors, savedColors, sizeof(pyramidColors));
    glBindBuffer(GL_ARRAY_BUFFER, vbo_pyramid[1]);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(pyramidColors), pyramidColors);
}

