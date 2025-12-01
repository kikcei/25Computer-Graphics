#pragma once
#include <vector>
#include <gl/glew.h>
#include <gl/glm/glm.hpp>
#include <gl/glm/gtc/matrix_transform.hpp>

extern GLuint textureID;
extern GLuint floorShaderID;
extern GLuint floorTexture;

class Floor {
public:
    glm::vec3 pos;     
    glm::vec3 size;    

    Floor(glm::vec3 p, glm::vec3 s);
    void Draw(const glm::mat4& view, const glm::mat4& proj);
};