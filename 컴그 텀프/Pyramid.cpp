#include "Pyramid.h"
#include "ShapeBuffer.h"

Pyramid::Pyramid(glm::vec3 p, glm::vec3 s)
{
    pos = p;
    size = s;
    rotation = glm::vec3(0.0f, 0.0f, 0.0f);      // 초기화는 무조건 해줘야됨
}

void Pyramid::Draw(const glm::mat4& view, const glm::mat4& proj, GLuint mvpLoc)
{
    glm::mat4 model = glm::mat4(1.0f);

    // 1. Translation
    model = glm::translate(model, pos);

    // 2. Rotation
    model = glm::rotate(model, glm::radians(rotation.x), glm::vec3(1, 0, 0));
    model = glm::rotate(model, glm::radians(rotation.y), glm::vec3(0, 1, 0));
    model = glm::rotate(model, glm::radians(rotation.z), glm::vec3(0, 0, 1));

    // 3. Scale
    model = glm::scale(model, size);

    glm::mat4 mvp = proj * view * model;
    glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, &mvp[0][0]);

    glBindVertexArray(vao_pyramid);
    glDrawElements(GL_TRIANGLES, 18, GL_UNSIGNED_INT, 0);
}