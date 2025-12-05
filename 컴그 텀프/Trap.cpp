#include "Trap.h"
#include "ShapeBuffer.h"

Trap::Trap(glm::vec3 p, glm::vec3 s)
{
    pos = p;
    size = s;
}

// ==============================
//  보트가 윗면을 지나가는지 체크
// ==============================
void Trap::Update(const glm::vec3& boatPos)
{
    float distX = fabs(boatPos.x - pos.x);
    float distZ = fabs(boatPos.z - pos.z);

    // 바닥 위에 올라온 상태인가 판정
    bool onTrap = (distX < size.x * 0.5f) && (distZ < size.z * 0.5f);

    if (onTrap)
    {
        // 가시 올라오는 중
        spikeHeight += 0.05f;
        if (spikeHeight > 1.0f) spikeHeight = 1.0f;
    }
    else
    {
        // 가시 내려가는 중
        spikeHeight -= 0.05f;
        if (spikeHeight < 0.0f) spikeHeight = 0.0f;
    }
}

void Trap::Draw(const glm::mat4& view, const glm::mat4& proj, GLuint mvpLoc)
{
    // --- 바닥 ---
    glm::mat4 base = glm::mat4(1.0f);
    base = glm::translate(base, pos);
    base = glm::scale(base, size);

    glm::mat4 mvp = proj * view * base;
    glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, &mvp[0][0]);

    glBindVertexArray(vao_trap);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    // --- 가시 ---
    if (spikeHeight > 0.0f)
    {
        glm::mat4 spikeModel = glm::mat4(1.0f);
        spikeModel = glm::translate(spikeModel, glm::vec3(pos.x, pos.y + spikeHeight, pos.z));
        spikeModel = glm::scale(spikeModel, glm::vec3(0.2f, 0.5f, 0.2f));

        glm::mat4 mvp2 = proj * view * spikeModel;
        glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, &mvp2[0][0]);

        glBindVertexArray(vao_pyramid);
        glDrawElements(GL_TRIANGLES, 18, GL_UNSIGNED_INT, 0);
    }
}