#include "Game_Manager.h"
#include "ShapeBuffer.h"

GameManager::GameManager()
    : basicFloor(glm::vec3(0, 0, -8), glm::vec3(5.0f, 0.3f, 20.0f))
{

}

void GameManager::LoadStage(int stage)
{
    // 기존 데이터 모두 초기화
    rotatingObstacle.clear();
    basicRight.clear();
    basicLeft.clear();

    if (stage == 1)
    {
        // Stage 1 장애물 배치
        basicRight.push_back(BasicObstacle_Right(glm::vec3(2, 0.1, 0), 3.0f, 0.5f));
        basicLeft.push_back(BasicObstacle_Left(glm::vec3(-2, 0.1, 0), 3.0f, 0.5f));

        basicRight.push_back(BasicObstacle_Right(glm::vec3(2, 0.1, -3), 3.0f, 0.5f));
        basicLeft.push_back(BasicObstacle_Left(glm::vec3(-2, 0.1, -3), 3.0f, 0.5f));

        // 회전형 장애물도 배치 가능
        // rotatingObstacle.emplace_back(glm::vec3(0,0,-5), 3.0f, 0.7f, 45.0f);
        //rotating_obstacle.push_back(PyramidWall(glm::vec3(0, 0, 0), 3.0f, 0.7f));       // push_back -> 동적으로 맨뒤에 요소 추가
        //rotating_obstacle.push_back(PyramidWall(glm::vec3(0, 0, 0), 3.0f, 0.7f, 90.0f));
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
    basicFloor.Draw(view, proj);

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