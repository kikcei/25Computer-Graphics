#pragma once
#include <vector>
#include <gl/glm/glm.hpp>

#include "PyramidWall.h"
#include "BasicObstacle.h"
#include "Floor.h"

class GameManager
{
public:
    Floor basicFloor;
    std::vector<PyramidWall> rotatingObstacle;
    std::vector<BasicObstacle_Right> basicRight;
    std::vector<BasicObstacle_Left> basicLeft;

    GameManager();
    void LoadStage(int stage);
    void Update(float dt);
    void Draw(const glm::mat4& view,const glm::mat4& proj,GLuint mvpLoc);
};