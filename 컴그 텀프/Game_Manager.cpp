#include "Game_Manager.h"
#include "ShapeBuffer.h"

GameManager::GameManager()
{
   
}

void GameManager::LoadStage(int stage)
{
    // 기존 데이터 모두 초기화
    stageObjects.clear();
    floors.clear();
    rotatingObstacle.clear();
    basicRight.clear();
    basicLeft.clear();

    if (stage == 1)
    {
        stageObjects = {
        { FLOOR, { 0,0,-8 }, 5.0f, 20.0f }, // Floor: pos, length(X), spacing(Z)
        { MOVE_RIGHT, { 2,0.1,0 },  3.0f, 0.5f },  // right move
        { MOVE_LEFT,  {-2,0.1,0 },  3.0f, 0.5f },  // left move
        { MOVE_RIGHT, { 2,0.1,-3 }, 3.0f, 0.5f },
        { MOVE_LEFT,  {-2,0.1,-3 }, 3.0f, 0.5f },
        { ROTATE,     { 0,0,-5 },   3.0f, 0.7f }   // rotate = length=3, spacing=0.7
        };
    }
    for (auto& o : stageObjects)
    {
        switch (o.type)
        {
        case 0: // FLOOR
            floors.emplace_back(o.pos, glm::vec3(o.length, 0.3f, o.spacing));
            break;

        case 1: // ROTATE
            rotatingObstacle.emplace_back(o.pos, o.length, o.spacing);
            break;

        case 2: // LEFT MOVE
            basicLeft.emplace_back(o.pos, o.length, o.spacing);
            break;

        case 3: // RIGHT MOVE
            basicRight.emplace_back(o.pos, o.length, o.spacing);
            break;
        }
    }
    // stage 2, 3 ... 추가 가능
}

void GameManager::Update(float dt)
{
    for (auto& w : rotatingObstacle)
        w.Update(dt);

    for (auto& w : basicRight)
        w.Update_Right(dt);

    for (auto& w : basicLeft)
        w.Update_Left(dt);
}

void GameManager::Draw(const glm::mat4& view, const glm::mat4& proj, GLuint mvpLoc)
{
    // 바닥 먼저 렌더링
    for (auto& f : floors)
        f.Draw(view, proj);

    // 회전형 장애물
    for (auto& w : rotatingObstacle)
        w.Draw(view, proj, mvpLoc);

    // 기본 장애물(오른쪽)
    for (auto& w : basicRight)
        w.Draw_Right(view, proj, mvpLoc);

    // 기본 장애물(왼쪽)
    for (auto& w : basicLeft)
        w.Draw_Left(view, proj, mvpLoc);
}