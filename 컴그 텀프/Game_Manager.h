#pragma once
#include <vector>
#include <gl/glm/glm.hpp>

#include "PyramidWall.h"
#include "BasicObstacle.h"
#include "Floor.h"

enum ObjType { FLOOR = 0, ROTATE = 1, MOVE_LEFT = 2, MOVE_RIGHT = 3 };

struct ObjectData
{
    ObjType type;
    glm::vec3 pos;
    float length;         // Ω∫∆Ω¿« ±Ê¿Ã
    float spacing;        // ª‘¿« ∞£∞›
};

class GameManager
{
public:
    std::vector<Floor> floors;
    std::vector<PyramidWall> rotatingObstacle;
    std::vector<BasicObstacle_Right> basicRight;
    std::vector<BasicObstacle_Left> basicLeft;

    std::vector<ObjectData> stageObjects;

    GameManager();
    void LoadStage(int stage);
    void Update(float dt);
    void Draw(const glm::mat4& view,const glm::mat4& proj,GLuint mvpLoc);
};

