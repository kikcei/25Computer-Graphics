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

        // -------- 상단 스파이크 (로컬 좌표) --------
        Pyramid top(
            glm::vec3(offset + spacing / 2, wallSize.y * 0.5f, 0.0f),
            glm::vec3(spikeSize)
        );
        top.rotation = glm::vec3(0, 0, 0);
        spikes.push_back(top);

        // -------- 하단 스파이크 (로컬 좌표) --------
        Pyramid bottom(
            glm::vec3(offset + spacing / 2, -wallSize.y * 0.5f, 0.0f),
            glm::vec3(spikeSize)
        );
        bottom.rotation = glm::vec3(180.0f, 0, 0);
        spikes.push_back(bottom);

        // -------- 오른쪽 --------
        Pyramid right(
            glm::vec3(wallSize.x * 0.5f, offset + spacing / 2, 0.0f),
            glm::vec3(spikeSize)
        );
        right.rotation = glm::vec3(0, 0, -90.0f);
        spikes.push_back(right);

        // -------- 왼쪽 --------
        Pyramid left(
            glm::vec3(-wallSize.x * 0.5f, offset + spacing / 2, 0.0f),
            glm::vec3(spikeSize)
        );
        left.rotation = glm::vec3(0, 0, 90.0f);
        spikes.push_back(left);
    }
}

void MoveWall::Draw(const glm::mat4& view, const glm::mat4& proj, GLuint mvpLoc)
{
    // (1) Pyramid 색상 백업
    GLfloat savedColors[5][3];
    memcpy(savedColors, pyramidColors, sizeof(pyramidColors));

    // (2) 색 설정
    UpdatePyramidColor(0.3f, 0.5f, 6.0f);

    // ===================================================
    // A. 스케일 없는 벽 모델 행렬
    // ===================================================
    glm::mat4 model_noScale = glm::mat4(1.0f);
    model_noScale = glm::translate(model_noScale, pos);
    model_noScale = glm::rotate(model_noScale, glm::radians(rotateAngle), glm::vec3(0, 1, 0));

    // ===================================================
    // B. 벽 출력 (scale 포함)
    // ===================================================
    glm::mat4 model = model_noScale;
    model = glm::scale(model, size);

    glm::mat4 mvp = proj * view * model;
    glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, &mvp[0][0]);

    glBindVertexArray(vao_movewall);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

    // ===================================================
    // C. 스파이크 출력 (scale 없는 벽 + 로컬 위치)
    // ===================================================
    for (auto& sp : spikes)
    {
        glm::mat4 sm = model_noScale;
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

    // (3) 색 복구
    memcpy(pyramidColors, savedColors, sizeof(pyramidColors));
    glBindBuffer(GL_ARRAY_BUFFER, vbo_pyramid[1]);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(pyramidColors), pyramidColors);
}

void MoveWall::SetMoveRange(glm::vec3 minPos, glm::vec3 maxPos, float speed)
{
    moveMin = minPos;
    moveMax = maxPos;
    moveSpeed = speed;
    moveDir = glm::vec3(1.0f);
}

void MoveWall::Update(float dt)
{
    pos += moveDir * moveSpeed * dt;

    if (pos.x > moveMax.x) { pos.x = moveMax.x; moveDir.x = -1; }
    if (pos.x < moveMin.x) { pos.x = moveMin.x; moveDir.x = 1; }

    if (pos.y > moveMax.y) { pos.y = moveMax.y; moveDir.y = -1; }
    if (pos.y < moveMin.y) { pos.y = moveMin.y; moveDir.y = 1; }

    if (pos.z > moveMax.z) { pos.z = moveMax.z; moveDir.z = -1; }
    if (pos.z < moveMin.z) { pos.z = moveMin.z; moveDir.z = 1; }
}
