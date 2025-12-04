#include "Skybox.h"
#include "ShapeBuffer.h"

GLuint skyShaderID;
GLuint skyTexture;


Skybox::Skybox(glm::vec3 p, glm::vec3 s)
{
    pos = p;           // 위치
    size = s;          // 크기
}

void Skybox::Draw(const glm::mat4& view, const glm::mat4& proj)
{
    glUseProgram(skyShaderID);

    // -----------------------------
    // 1) Model 행렬 설정
    // -----------------------------
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, pos);
    model = glm::scale(model, size);

    glm::mat4 mvp = proj * view * model;

    GLuint mvpLoc = glGetUniformLocation(skyShaderID, "mvp");
    glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, &mvp[0][0]);


    // -----------------------------
    // 2) 텍스처 바인딩
    // -----------------------------
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, skyTexture);
    glUniform1i(glGetUniformLocation(skyShaderID, "skyTex"), 0);


    // -----------------------------
    // 3) VAO 바인딩 & Draw
    // -----------------------------
    glBindVertexArray(vao_skybox);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

    // -----------------------------
    // 4) 정리
    // -----------------------------
    glBindTexture(GL_TEXTURE_2D, 0);
    glUseProgram(0);
}