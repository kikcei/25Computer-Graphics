#include "Stick.h"
#include "ShapeBuffer.h"

Stick::Stick(glm::vec3 p, glm::vec3 s)
{
    pos = p;           // 위치
    size = s;          // 크기
}

void Stick::Draw(const glm::mat4& view, const glm::mat4& proj, GLuint mvpLoc)
{
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, pos);
    model = glm::scale(model, size);

    glm::mat4 mvp = proj * view * model;
    glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, &mvp[0][0]);

    glBindVertexArray(vao_stick);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
}