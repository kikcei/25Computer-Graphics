#pragma once
#include <vector>
#include <gl/glm/glm.hpp>

#include "PyramidWall.h"
#include "BasicObstacle.h"
#include "Floor.h"
#include "Skybox.h"

enum ObjType { FLOOR = 0, ROTATE = 1, MOVE_LEFT = 2, MOVE_RIGHT = 3 };

struct ObjectData
{
    ObjType type;
    glm::vec3 pos;
    float length;         // 스틱의 길이
    float spacing;        // 뿔의 간격
    glm::vec3 axis = glm::vec3(0, 0, 1); // 회전 장애물 회전축
    float angle;          // 회전 장애물 회전각도
    float basicangle = 0.0f;
    glm::vec3 basicaxis = glm::vec3(0, 0, 0); // 기본 도형 회전
    float PM_Y = 0.2f;
    float PM_Z = 0.2f;
    float PyramidHeight = 0.2f;
    int rotateDir = 1;
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

