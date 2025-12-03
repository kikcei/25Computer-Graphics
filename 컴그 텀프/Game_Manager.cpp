#include "Game_Manager.h"
#include "ShapeBuffer.h"

GameManager::GameManager() 
{
   
}

// glm::vec3 pos;
// float length;         // 스틱의 길이
// float spacing;        // 뿔의 간격
// glm::vec3 axis = glm::vec3(0, 0, 1); // 회전 장애물 회전축
// float angle;          // 회전 장애물 회전각도
// float basicangle = 0.0f; // 기본 장애물 회전
// glm::vec3 basicaxis = glm::vec3(0, 0, 0); // 기본 도형 회전

void GameManager::LoadStage(int stage)
{
    // 기존 데이터 모두 초기화
    skyboxes.emplace_back(
        glm::vec3(0, 3.0f, -30.0f),   // 위치
        glm::vec3(60.0f, 40.0f, 1.0f) // 크기(화면보다 크게)
    );

    stageObjects.clear();
    floors.clear();
    rotatingObstacle.clear();
    basicRight.clear();
    basicLeft.clear();

    if (stage == 1)
    {
        stageObjects = {
//-------------------------------------------------------------------------------
        { FLOOR, { 0,0,0}, 5.0f, 5.0f }, // Floor: pos, length(X), spacing(Z)
        { FLOOR, { 0,0,-5}, 5.0f, 5.0f },
        { FLOOR, { 0,0,-10}, 5.0f, 5.0f },
        { FLOOR, { 0,0,-15}, 5.0f, 5.0f },
        { FLOOR, { 0,0,-20}, 5.0f, 5.0f },

        { FLOOR, { 0,0,-20}, 5.0f, 5.0f },
        { FLOOR, { 5,0,-20}, 5.0f, 5.0f },
        { FLOOR, {-5,0,-20}, 5.0f, 5.0f },
        { FLOOR, { 0,0,-25}, 5.0f, 5.0f },
        { FLOOR, { 5,0,-25}, 5.0f, 5.0f },
        { FLOOR, {-5,0,-25}, 5.0f, 5.0f },
        { FLOOR, { 0,0,-30}, 5.0f, 5.0f },
        { FLOOR, { 5,0,-30}, 5.0f, 5.0f },
        { FLOOR, {-5,0,-30}, 5.0f, 5.0f },
        { FLOOR, { 0,0,-35}, 5.0f, 5.0f },
        { FLOOR, { 5,0,-35}, 5.0f, 5.0f },
        { FLOOR, {-5,0,-35}, 5.0f, 5.0f },


       
        { FLOOR, { 0,0,-40}, 5.0f, 5.0f },
        { FLOOR, { 0,0,-45}, 5.0f, 5.0f },
        { FLOOR, { 0,0,-50}, 5.0f, 5.0f },
        { FLOOR, { 0,0,-55}, 5.0f, 5.0f },
        { FLOOR, { 0,0,-60}, 5.0f, 5.0f },
        { FLOOR, { 0,0,-65}, 5.0f, 5.0f },
        { FLOOR, { 0,0,-70}, 5.0f, 5.0f },
//-------------------------------------------------------------------------------
        { MOVE_RIGHT, { 2.2,0.1,0 },  3.0f, 0.5f },  // right move
        { MOVE_LEFT,  {-2.2,0.1,0 },  3.0f, 0.5f },  // left move

        { MOVE_LEFT, { 2.2,0.1,-3.0 }, 3.0f, 0.5f },
        { MOVE_RIGHT,  {-2.2,0.1,-3.0 }, 3.0f, 0.5f },

        { MOVE_RIGHT, { 2.2,0.1,-6.0 },  3.0f, 0.5f }, 
        { MOVE_LEFT,  {-2.2,0.1,-6.0 },  3.0f, 0.5f }, 

        { MOVE_RIGHT, { 2.2,0.1,-15.0 },  3.0f, 0.5f },
        { MOVE_LEFT,  {-2.2,0.1,-15.0 },  3.0f, 0.5f },

        { MOVE_LEFT, { 2.2,0.1,-18.0 },  3.0f, 0.5f },
        { MOVE_RIGHT,  {-2.2,0.1,-18.0 },  3.0f, 0.5f },

//-------------------------------------------------------------------------------
        { ROTATE,
         {0, 1.5f, -9.9f}, // pos
         4.0f,             // length
         0.6f,             // spacing
         {0,0,1},          // axis (애니메이션 회전축)
         0.0f,             // angle (초기 애니메이션 각도)
         0.0f,             // basicangle (기본배치 회전)
         {0,0,1}           // basicaxis (기본배치 회전축)
        },

        { ROTATE,
        {0, 1.5f, -10},   // pos
         4.0f,           // length
         0.6f,           // spacing
         {0,0,1},        // axis
         90.0f,          // angle
         0.0f,           // basicangle
         {0,0,1}         // basicaxis
        },

         { ROTATE,{-3,0.1,-31}, 9.0f, 1.0f, {0,0,1}, 0.0f,90.0f,{1,0,0},0.3,0.3 , 0.6f ,-1},
         { ROTATE,{-3,0.1,-31}, 9.0f, 1.0f, {0,0,1}, 90.0f,90.0f,{1,0,0},0.3,0.3, 0.6f ,-1},

         { ROTATE,{3,0.1,-24}, 9.0f, 1.0f, {0,0,1}, 0.0f,90.0f,{1,0,0},0.3,0.3 , 0.6f,1},
         { ROTATE,{3,0.1,-24}, 9.0f, 1.0f, {0,0,1}, 90.0f,90.0f,{1,0,0},0.3,0.3, 0.6f,1}


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
            rotatingObstacle.emplace_back(o.pos, o.length, o.spacing, o.basicaxis, o.axis, o.basicangle, o.angle, o.PM_Y, o.PM_Z, o.PyramidHeight, o.rotateDir);
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
    glDepthMask(GL_FALSE);  // 깊이 쓰기 OFF

    for (auto& sky : skyboxes)
        sky.Draw(view, proj);

    glDepthMask(GL_TRUE);   // 다시 ON

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