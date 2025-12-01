#include "Floor.h"
#include "ShapeBuffer.h"

GLuint floorShaderID;
GLuint floorTexture;


Floor::Floor(glm::vec3 p, glm::vec3 s)
{
    pos = p;           // 위치
    size = s;          // 크기
}

void Floor::Draw(const glm::mat4& view, const glm::mat4& proj)
{
    glUseProgram(floorShaderID);

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, pos);
    model = glm::scale(model, size);

    glm::mat4 mvp = proj * view * model;
    GLuint mvpLoc = glGetUniformLocation(shaderProgramID, "mvp");
    glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, &mvp[0][0]);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, floorTexture);

    glBindVertexArray(vao_floor);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    glBindTexture(GL_TEXTURE_2D, 0);
    glUseProgram(shaderProgramID);

}